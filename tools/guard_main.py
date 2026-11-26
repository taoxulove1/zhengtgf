#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""guard_main.py — 免费私有库下替代分支保护的"事后硬守门"。

GitHub 免费私有库不开放 branch protection / rulesets（需 Pro 或 public），
于是把四条硬约束搬到 push 之后自动判定，违规即回滚：

  R1 提交信息符合 Conventional Commits
  R2 线性历史：本次 push 范围不得含 merge commit
  R3 单文件 ≤ 100 MB
  R4 未重写历史：BEFORE 必须是 HEAD 的祖先（等价于禁 force push）

用法:
    python tools/guard_main.py --before <sha> --after <sha> [--json]

退出码: 0 通过 / 1 违规 / 2 参数错误
"""

import argparse
import json
import re
import subprocess
import sys

CONV = re.compile(r"^(feat|fix|data|docs|chore|refactor|perf|test|revert)(\([a-z0-9-]+\))?: .{4,60}$")
SKIP = re.compile(r"\[skip ci\]", re.I)


def git(*args):
    return subprocess.run(["git", *args], capture_output=True, text=True).stdout


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--before", required=True)
    ap.add_argument("--after", required=True)
    ap.add_argument("--json", action="store_true")
    a = ap.parse_args()

    if set(a.before) == {"0"} or not a.before:
        print(json.dumps({"ok": True, "note": "首次推送，跳过"}, ensure_ascii=False))
        return 0

    v = []

    # R4 祖先性（等价禁 force push）
    anc = subprocess.run(["git", "merge-base", "--is-ancestor", a.before, a.after]).returncode == 0
    if not anc:
        v.append({"rule": "R4", "detail": f"{a.before[:8]} 不是 {a.after[:8]} 的祖先，历史被重写"})

    rng = f"{a.before}..{a.after}"

    # R2 线性历史
    merges = [l for l in git("log", "--merges", "--pretty=%h %s", rng).splitlines() if l.strip()]
    for m in merges:
        v.append({"rule": "R2", "detail": f"merge commit: {m}"})

    # R1 提交信息
    for line in git("log", "--no-merges", "--pretty=%h%x09%s", rng).splitlines():
        if not line.strip():
            continue
        h, _, subj = line.partition("\t")
        if SKIP.search(subj):
            continue
        if not CONV.match(subj):
            v.append({"rule": "R1", "detail": f"{h} 提交信息不合规: {subj}"})

    # R3 体积
    big = []
    for line in git("ls-tree", "-r", "-l", a.after).splitlines():
        parts = line.split()
        if len(parts) < 5 or parts[1] != "blob":
            continue
        size = int(parts[3])
        if size > 100 * 1024 * 1024:
            big.append((parts[4], size))
    for path, size in big:
        v.append({"rule": "R3", "detail": f"{path} = {size / 1048576:.1f} MB > 100 MB"})

    ok = not v
    if a.json:
        print(json.dumps({"ok": ok, "violations": v}, ensure_ascii=False, indent=2))
    else:
        print(f"守门: {'通过' if ok else f'违规 {len(v)} 项'}")
        for x in v:
            print(f"  [{x['rule']}] {x['detail']}")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
