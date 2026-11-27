#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""remote.py —— 目标服务器 SSH 执行 / 上传 / 下载（凭据不进仓库）。

凭据优先级：
    环境变量 ZT_SSH_HOST / ZT_SSH_USER / ZT_SSH_PASS
    本地文件 .secrets/server.local.json  {"host":..,"user":..,"password":..}

用法：
    python tools/remote.py run "ss -lntp"
    python tools/remote.py put tools/capture-server.sh /root/capture.sh
    python tools/remote.py get /root/zt/server/config.xml state/config.remote.xml
    python tools/remote.py capture 20 captures/win.pcapng "tcp and port 8000"
"""

import argparse
import json
import os
import sys

CRED = ".secrets/server.local.json"


def cred():
    host = os.environ.get("ZT_SSH_HOST", "")
    user = os.environ.get("ZT_SSH_USER", "")
    pwd = os.environ.get("ZT_SSH_PASS", "")
    if not (host and user and pwd) and os.path.exists(CRED):
        d = json.load(open(CRED, encoding="utf-8"))
        host, user, pwd = host or d["host"], user or d["user"], pwd or d["password"]
    if not (host and user and pwd):
        raise SystemExit(f"[ERR] 缺凭据：设 ZT_SSH_* 或写 {CRED}")
    return host, user, pwd


def client():
    import paramiko
    host, user, pwd = cred()
    c = paramiko.SSHClient()
    c.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    c.connect(hostname=host, port=22, username=user, password=pwd, timeout=12,
              allow_agent=False, look_for_keys=False, banner_timeout=12, auth_timeout=12)
    return c


def run(c, cmd, timeout=120):
    si, so, se = c.exec_command(cmd, timeout=timeout)
    out = so.read().decode("utf-8", "replace")
    err = se.read().decode("utf-8", "replace")
    rc = so.channel.recv_exit_status()
    return out, err, rc


def put_b64(c, local, remote, chmod=True, timeout=120):
    """不经 SFTP 落地文件：本地 base64 -> SSH stdin -> `base64 -d > remote`。

    适用场景：目标机 SFTP 子系统被禁 / /root 权限位异常导致 SSH_FXP_OPEN 返回 ENOENT，
    而 SSH exec 通道正常。脚本类小文件（<1MB）走这条路最稳。
    """
    import base64
    import shlex

    data = base64.b64encode(open(local, "rb").read())
    q = shlex.quote(remote)
    cmd = f"base64 -d > {q}"
    if chmod:
        cmd += f" && chmod +x {q}"
    cmd += f" && ls -l {q}"
    si, so, se = c.exec_command(cmd, timeout=timeout)
    step = 32768
    for i in range(0, len(data), step):
        si.write(data[i:i + step].decode())
        si.flush()
    si.channel.shutdown_write()
    out = so.read().decode("utf-8", "replace")
    err = se.read().decode("utf-8", "replace")
    rc = so.channel.recv_exit_status()
    if rc != 0:
        raise SystemExit(f"[ERR] push {local} -> {remote} rc={rc} {err.strip()}")
    return (out + err).strip()


def main():
    ap = argparse.ArgumentParser()
    sub = ap.add_subparsers(dest="cmd", required=True)

    p = sub.add_parser("run"); p.add_argument("command"); p.add_argument("--timeout", type=int, default=120)
    p = sub.add_parser("put"); p.add_argument("local"); p.add_argument("remote")
    p = sub.add_parser("get"); p.add_argument("remote"); p.add_argument("local")
    p = sub.add_parser("push")
    p.add_argument("local")
    p.add_argument("remote")
    p.add_argument("--no-chmod", action="store_true")
    p = sub.add_parser("capture")
    p.add_argument("seconds", type=int)
    p.add_argument("local_out")
    p.add_argument("bpf", nargs="?", default="tcp")
    a = ap.parse_args()

    c = client()
    try:
        if a.cmd == "run":
            out, err, rc = run(c, a.command, a.timeout)
            sys.stdout.write(out)
            if err.strip():
                sys.stderr.write("\n[stderr]\n" + err)
            return rc
        if a.cmd == "put":
            sf = c.open_sftp(); sf.put(a.local, a.remote); sf.close()
            print(f"put {a.local} -> {a.remote}")
            return 0
        if a.cmd == "push":
            print("push " + put_b64(c, a.local, a.remote, chmod=not a.no_chmod))
            return 0
        if a.cmd == "get":
            os.makedirs(os.path.dirname(a.local) or ".", exist_ok=True)
            sf = c.open_sftp(); sf.get(a.remote, a.local); sf.close()
            print(f"get {a.remote} -> {a.local} ({os.path.getsize(a.local)} B)")
            return 0
        if a.cmd == "capture":
            remote = f"/tmp/ztcap_{os.getpid()}.pcapng"
            cmd = (f"nohup timeout {a.seconds} /usr/sbin/tcpdump -i any -n -s0 "
                   f"-w {remote} '{a.bpf}' >/tmp/ztcap.log 2>&1 & sleep {a.seconds + 3}; "
                   f"ls -l {remote}; rm -f /tmp/ztcap.log")
            out, err, _ = run(c, cmd, timeout=a.seconds + 40)
            print(out.strip() or err.strip())
            os.makedirs(os.path.dirname(a.local_out) or ".", exist_ok=True)
            sf = c.open_sftp(); sf.get(remote, a.local_out); sf.close()
            print(f"get {remote} -> {a.local_out} ({os.path.getsize(a.local_out)} B)")
            run(c, f"rm -f {remote}")
            return 0
    finally:
        c.close()
    return 0


if __name__ == "__main__":
    sys.exit(main())
