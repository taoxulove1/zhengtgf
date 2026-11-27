#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""server_probe.py —— 目标 Linux 服务器环境探针（P4B 部署前置）。

凭据来源（按优先级，均不进仓库）：
    环境变量  ZT_SSH_PASS / ZT_SSH_USER / ZT_SSH_HOST
    本地文件  .secrets/server.local.json   （已在 .gitignore 内，形如
              {"host":"...","user":"root","password":"..."}）

用法：
    python tools/server_probe.py                # 全量探针，人读
    python tools/server_probe.py --json         # 机读，写 state/server-env.json
    python tools/server_probe.py --cmd "uname -a"   # 单命令
"""

import argparse
import io
import json
import os
import sys

LOCAL_CRED = ".secrets/server.local.json"

# 探针分三组，任何一条失败都不中断，只记 err
GROUPS = {
    "os": [
        ("os_release", "cat /etc/os-release 2>/dev/null | head -20"),
        ("kernel", "uname -a"),
        ("glibc", "ldd --version 2>&1 | head -1"),
        ("arch", "uname -m"),
        ("timezone", "timedatectl 2>/dev/null | head -5 || date"),
    ],
    "hw": [
        ("cpu", "nproc; grep -m1 'model name' /proc/cpuinfo"),
        ("mem", "free -m"),
        ("disk", "df -hT / /home 2>/dev/null"),
        ("load", "uptime"),
    ],
    "runtime": [
        ("pkgmgr", "which yum dnf apt 2>/dev/null; rpm -q centos-release 2>/dev/null"),
        ("gcc", "gcc --version 2>&1 | head -1; g++ --version 2>&1 | head -1"),
        ("make", "make --version 2>&1 | head -1"),
        ("java", "java -version 2>&1 | head -2"),
        ("python", "python3 -V 2>&1; python -V 2>&1"),
        ("mysql", "mysql --version 2>&1; systemctl is-active mysqld mariadb 2>&1"),
        ("dotnet", "dotnet --info 2>&1 | head -3"),
    ],
    "net": [
        ("listen", "ss -lntp 2>/dev/null || netstat -lntp 2>/dev/null"),
        ("firewall", "systemctl is-active firewalld 2>&1; iptables -S 2>/dev/null | head -20"),
        ("selinux", "getenforce 2>/dev/null"),
        ("public_ip", "curl -s --max-time 5 ifconfig.me 2>/dev/null || echo n/a"),
    ],
    "game": [
        ("existing", "ls -la /home /opt /data /root 2>/dev/null | head -40"),
        ("zt_proc", "ps -ef | grep -iE 'scenes|gateway|session|record|bill|super|mini' | grep -v grep"),
        ("ports_1xxxx", "ss -lntp 2>/dev/null | grep -E ':(700[0-9]|1000[0-9])' || echo none"),
        ("libs", "ls /usr/lib64/libstdc++.so* /usr/lib/x86_64-linux-gnu/libstdc++.so* 2>/dev/null"),
    ],
}


def load_cred():
    host = os.environ.get("ZT_SSH_HOST", "")
    user = os.environ.get("ZT_SSH_USER", "")
    pwd = os.environ.get("ZT_SSH_PASS", "")
    if not (host and user and pwd) and os.path.exists(LOCAL_CRED):
        with open(LOCAL_CRED, encoding="utf-8") as f:
            d = json.load(f)
        host = host or d.get("host", "")
        user = user or d.get("user", "")
        pwd = pwd or d.get("password", "")
    return host, user, pwd


def connect(host, user, pwd, timeout=10):
    import paramiko
    c = paramiko.SSHClient()
    c.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    c.connect(hostname=host, port=22, username=user, password=pwd,
              timeout=timeout, allow_agent=False, look_for_keys=False,
              banner_timeout=timeout, auth_timeout=timeout)
    return c


def run(c, cmd):
    try:
        _, out, err = c.exec_command(cmd, timeout=20)
        o = out.read().decode("utf-8", "replace").strip()
        e = err.read().decode("utf-8", "replace").strip()
        return (o + ("\n" + e if e else "")).strip()
    except Exception as ex:
        return f"[err] {type(ex).__name__}: {ex}"


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--json", action="store_true")
    ap.add_argument("--cmd", default="")
    ap.add_argument("--out", default="state/server-env.json")
    a = ap.parse_args()

    host, user, pwd = load_cred()
    if not (host and user and pwd):
        print(f"[ERR] 缺凭据：设 ZT_SSH_HOST/USER/PASS，或写 {LOCAL_CRED}", file=sys.stderr)
        return 2

    try:
        c = connect(host, user, pwd)
    except Exception as e:
        print(f"[ERR] SSH 连接失败 {user}@{host}: {type(e).__name__}: {e}", file=sys.stderr)
        return 3

    if a.cmd:
        print(run(c, a.cmd))
        c.close()
        return 0

    result = {"host": host, "user": user, "groups": {}}
    human = io.StringIO()
    for gname, items in GROUPS.items():
        result["groups"][gname] = {}
        human.write(f"\n===== {gname} =====\n")
        for label, cmd in items:
            out = run(c, cmd)
            result["groups"][gname][label] = out
            human.write(f"--- {label}  [{cmd}]\n{out}\n")
    c.close()

    if a.json:
        os.makedirs(os.path.dirname(a.out), exist_ok=True)
        with open(a.out, "w", encoding="utf-8") as f:
            json.dump(result, f, ensure_ascii=False, indent=2)
        print(f"已写 {a.out}")
    else:
        sys.stdout.write(human.getvalue())
    return 0


if __name__ == "__main__":
    sys.exit(main())
