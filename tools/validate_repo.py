#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""validate_repo.py — 仓库规则与阶段门禁校验（CI 与本地同一套判据）。

用法:
    python tools/validate_repo.py --ci        # 通用规则：体积/JSON/DAG/命名/密钥
    python tools/validate_repo.py --manifest  # 资产清单自洽性
    python tools/validate_repo.py --phase P0  # 单阶段门禁
    python tools/validate_repo.py --all       # 全部

退出码: 0 通过 / 1 有失败项 / 2 参数错误
"""

import argparse
import json
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
MB = 1024 * 1024
MAX_TRACKED = 100 * MB

FAIL = []
PASS = []


def ok(msg):
    PASS.append(msg)


def bad(msg):
    FAIL.append(msg)


def read(path):
    p = os.path.join(ROOT, path)
    if not os.path.exists(p):
        return None
    with open(p, encoding="utf-8", errors="replace") as f:
        return f.read()


def count_rows(path):
    t = read(path)
    if t is None:
        return -1
    lines = [l for l in t.splitlines() if l.strip() and not l.strip().startswith("#")]
    return max(0, len(lines) - 1)


def count_lines(path):
    t = read(path)
    return 0 if t is None else len(t.splitlines())


def tracked_files():
    out = subprocess.run(["git", "ls-files", "-z"], cwd=ROOT,
                         capture_output=True).stdout
    return [p.decode("utf-8", "replace") for p in out.split(b"\0") if p]


# ---------------- 通用规则 ----------------
def check_ci():
    # 1) 单文件 100MB 硬限
    big = []
    for rel in tracked_files():
        p = os.path.join(ROOT, rel)
        if os.path.exists(p) and os.path.getsize(p) > MAX_TRACKED:
            big.append((rel, os.path.getsize(p)))
    if big:
        for rel, s in big[:10]:
            bad(f"[体积] {rel} = {s / MB:.1f} MB > 100 MB 硬限")
    else:
        ok("单文件体积全部 < 100 MB")

    # 2) JSON 可解析
    for rel in ("tasks/registry.json", "state/queue.json", "assets/manifest/assets.json",
                "config/variables.json", "config/schema/task.schema.json"):
        p = os.path.join(ROOT, rel)
        if not os.path.exists(p):
            bad(f"[缺失] {rel}")
            continue
        try:
            with open(p, encoding="utf-8") as f:
                json.load(f)
            ok(f"JSON 合法: {rel}")
        except Exception as e:
            bad(f"[JSON] {rel} 解析失败: {e}")

    # 3) 任务 DAG
    try:
        reg = json.loads(read("tasks/registry.json") or "{}")
        ids = [t["id"] for t in reg.get("tasks", [])]
        if len(ids) != len(set(ids)):
            bad("[DAG] 任务 ID 重复")
        else:
            ok(f"DAG 任务数 {len(ids)}，ID 唯一")
        for t in reg.get("tasks", []):
            for d in t.get("deps", []):
                if d not in ids:
                    bad(f"[DAG] {t['id']} 依赖不存在 {d}")
    except Exception as e:
        bad(f"[DAG] 解析失败: {e}")

    # 4) 分支命名（CI 环境）
    head = os.environ.get("GITHUB_HEAD_REF", "")
    if head and head != "main":
        if not re.match(r"^(feat|fix|data|chore|docs)/[a-z0-9][a-z0-9-]{2,40}$", head):
            bad(f"[分支] 命名不合规: {head}")
        else:
            ok(f"分支命名合规: {head}")

    # 5) 提交信息（PR 场景取最近一次提交）
    if os.environ.get("GITHUB_EVENT_NAME") == "pull_request":
        out = subprocess.run(["git", "log", "-1", "--pretty=%s"], cwd=ROOT,
                             capture_output=True, text=True).stdout.strip()
        if not re.match(r"^(feat|fix|data|docs|chore|refactor|perf|test|revert)(\([a-z-]+\))?: .{4,50}$", out):
            bad(f"[提交] 不符合 Conventional Commits: {out}")
        else:
            ok(f"提交信息合规: {out}")

    # 6) 敏感文件与明文密钥
    for rel in tracked_files():
        if re.search(r"(^|/)\.env(\.|$)", rel) or rel.endswith(".secret"):
            bad(f"[密钥] 疑似敏感文件入库: {rel}")
    key_pat = re.compile(r"(password|passwd|token|secret)\s*=\s*['\"]?[A-Za-z0-9_\-]{8,}", re.I)
    for rel in ("config/variables.json", "config/server/game.ini", "config/server/login.ini"):
        t = read(rel)
        if t and key_pat.search(t):
            bad(f"[密钥] {rel} 含疑似明文口令，改走 .env / Secrets")
    ok("敏感文件与明文密钥扫描完成")


# ---------------- 清单自洽 ----------------
def check_manifest():
    doc = json.loads(read("assets/manifest/assets.json") or "{}")
    files = doc.get("files", [])
    if not files:
        bad("[清单] assets.json 为空")
        return
    seen = set()
    for r in files:
        if r["path"] in seen:
            bad(f"[清单] 路径重复: {r['path']}")
        seen.add(r["path"])
        if not re.fullmatch(r"[0-9a-f]{64}", r.get("sha256", "")):
            bad(f"[清单] sha256 非法: {r['path']}")
        if r["size"] >= 100 * MB and r["route"] != "release-split":
            bad(f"[清单] 分层错误: {r['path']} {r['size'] / MB:.1f}MB 应为 release-split")
    if doc.get("file_count") != len(files):
        bad(f"[清单] file_count={doc.get('file_count')} 与条目数 {len(files)} 不符")
    if not FAIL:
        ok(f"清单自洽: {len(files)} 条，合计 {doc.get('total_bytes', 0) / MB:.1f} MB")


# ---------------- 阶段门禁 ----------------
PHASE_RULES = {
    "P0": {
        "files": ["docs/intake/ledger.md", "config/variables.json", "assets/manifest/assets.json"],
        "checks": [
            ("docs/intake/ledger.md", lambda: count_lines("docs/intake/ledger.md") >= 20,
             "台账 ≥ 20 行"),
            ("config/variables.json", lambda: len(json.loads(read("config/variables.json") or "{}").get("variables", {})) >= 15,
             "变量表 ≥ 15 项"),
            ("assets/manifest/assets.json", lambda: json.loads(read("assets/manifest/assets.json") or "{}").get("file_count", 0) >= 580,
             "资产清单 ≥ 580 条"),
            ("config/variables.json",
             lambda: json.loads(read("config/variables.json"))["variables"]["CLIENT_BACKUP_MD5"]["value"] != "TODO",
             "客户端整包冷备 MD5 已登记"),
            ("config/variables.json",
             lambda: json.loads(read("config/variables.json"))["variables"]["INTRANET_CIDR"]["status"] == "confirmed",
             "内网网段已确认（非假设）"),
        ],
    },
    "P1": {
        "files": ["tools/decrypt.py", "docs/protocol/msgid.csv"],
        "checks": [
            ("docs/protocol/msgid.csv", lambda: count_rows("docs/protocol/msgid.csv") >= 60,
             "msgid.csv ≥ 60 条"),
            ("captures/", lambda: os.path.isdir(os.path.join(ROOT, "captures")) and
             len([f for f in os.listdir(os.path.join(ROOT, "captures")) if f.endswith((".pcap", ".pcapng"))]) >= 1,
             "captures/ 至少 1 个 pcapng"),
        ],
    },
    "P2": {
        "files": ["docs/protocol/protocol.md"],
        "checks": [
            ("docs/protocol/protocol.md", lambda: count_lines("docs/protocol/protocol.md") >= 120,
             "协议表 ≥ 120 行"),
        ] + [
            ("docs/protocol/protocol.md", (lambda kw: lambda: kw in (read("docs/protocol/protocol.md") or ""))(kw),
             f"含「{kw}」类消息") for kw in ("登录", "选区", "进图", "心跳", "移动", "背包")
        ],
    },
    "P3": {
        "files": ["src/server-rebuild/Makefile"],
        "checks": [
            ("src/server-rebuild/", lambda: os.path.isdir(os.path.join(ROOT, "src/server-rebuild")) and
             len([f for f in os.listdir(os.path.join(ROOT, "src/server-rebuild")) if f.endswith((".cpp", ".h"))]) >= 5,
             "骨架 ≥ 5 个源文件"),
        ],
    },
    "P4A": {
        "files": ["docs/db/baseline.md", "sql/schema.sql"],
        "checks": [
            ("docs/db/baseline.md", lambda: count_lines("docs/db/baseline.md") >= 30, "库基线表 ≥ 30 行"),
            ("sql/schema.sql", lambda: count_lines("sql/schema.sql") >= 10, "schema.sql ≥ 10 行"),
        ],
    },
    "P4B": {
        "files": ["docs/deploy/runbook.md"],
        "checks": [
            ("docs/deploy/runbook.md", lambda: count_lines("docs/deploy/runbook.md") >= 40, "runbook ≥ 40 行"),
            ("deploy/systemd/", lambda: os.path.isdir(os.path.join(ROOT, "deploy/systemd")) and
             len(os.listdir(os.path.join(ROOT, "deploy/systemd"))) >= 1, "systemd unit ≥ 1 个"),
        ],
    },
    "P5": {
        "files": ["docs/qa/report.md", "docs/qa/cases.csv"],
        "checks": [
            ("docs/qa/cases.csv", lambda: count_rows("docs/qa/cases.csv") >= 32, "用例 ≥ 32 条"),
            ("docs/qa/report.md", lambda: count_lines("docs/qa/report.md") >= 40, "报告 ≥ 40 行"),
        ],
    },
    "P6": {
        "files": ["docs/delivery/checklist.md"],
        "checks": [
            ("docs/delivery/checklist.md", lambda: count_lines("docs/delivery/checklist.md") >= 30, "交付清单 ≥ 30 行"),
        ],
    },
}


def check_phase(pid):
    rules = PHASE_RULES.get(pid)
    if not rules:
        bad(f"[门禁] 未知阶段 {pid}")
        return
    for rel in rules["files"]:
        if not os.path.exists(os.path.join(ROOT, rel)):
            bad(f"[{pid}] 缺交付物: {rel}")
    for rel, fn, desc in rules["checks"]:
        try:
            if fn():
                ok(f"[{pid}] {desc} ✓")
            else:
                bad(f"[{pid}] 未达标: {desc}")
        except Exception as e:
            bad(f"[{pid}] {desc} 检查异常: {e}")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--ci", action="store_true")
    ap.add_argument("--manifest", action="store_true")
    ap.add_argument("--phase", choices=list(PHASE_RULES))
    ap.add_argument("--all", action="store_true")
    args = ap.parse_args()
    if not any([args.ci, args.manifest, args.phase, args.all]):
        args.ci = True

    if args.all or args.ci:
        check_ci()
    if args.all or args.manifest:
        check_manifest()
    if args.all:
        for p in PHASE_RULES:
            check_phase(p)
    elif args.phase:
        check_phase(args.phase)

    print(f"通过 {len(PASS)} 项 / 失败 {len(FAIL)} 项")
    for m in FAIL:
        print("  FAIL", m)
    return 0 if not FAIL else 1


if __name__ == "__main__":
    sys.exit(main())
