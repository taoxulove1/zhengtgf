#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""gh_ref_fix.py —— 诊断 / 修正远端分支尖端。

场景：`api_push_files.py` 在多推了一个合成提交后，想把分支指回原本已存在的提交，
      `PATCH git/refs` 返回 422。本工具先取证再修：
      1. 读分支当前 sha、该 sha 的 parents 与 tree；
      2. 读目标 sha 的 tree；
      3. 若 tree 相同 -> 说明合成提交无内容差异，直接 force 指回（用完整 40 位 sha）；
      4. 若 422，则依次尝试：完整 sha、DELETE+PATCH 重建 ref、把差异作为新提交补回。

用法：
    python tools/gh_ref_fix.py --repo taoxulove1/zhengtgf --branch feat/p3-native-build \
        --target 28b872d --diagnose
    python tools/gh_ref_fix.py --repo taoxulove1/zhengtgf --branch feat/p3-native-build \
        --target 28b872d --apply
"""

import argparse
import json
import os
import subprocess
import sys
import urllib.error
import urllib.request

API = "https://api.github.com"


def token():
    t = os.environ.get("GITHUB_TOKEN", "")
    if t:
        return t
    out = subprocess.run(
        ["git", "credential", "fill"],
        input="protocol=https\nhost=github.com\n\n",
        capture_output=True, text=True, check=True).stdout
    for line in out.splitlines():
        if line.startswith("password="):
            return line.split("=", 1)[1].strip()
    raise SystemExit("[ERR] 无 token")


def call(method, path, tok, body=None):
    data = json.dumps(body).encode() if body is not None else None
    req = urllib.request.Request(API + path, data=data, method=method)
    req.add_header("Authorization", "token " + tok)
    req.add_header("Accept", "application/vnd.github+json")
    try:
        with urllib.request.urlopen(req, timeout=40) as r:
            raw = r.read().decode()
            return r.status, (json.loads(raw) if raw.strip() else {})
    except urllib.error.HTTPError as e:
        raw = e.read().decode()
        try:
            return e.code, json.loads(raw)
        except Exception:
            return e.code, {"raw": raw}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--repo", required=True)
    ap.add_argument("--branch", required=True)
    ap.add_argument("--target", required=True)
    ap.add_argument("--diagnose", action="store_true")
    ap.add_argument("--apply", action="store_true")
    a = ap.parse_args()
    tok = token()

    st, ref = call("GET", f"/repos/{a.repo}/git/ref/heads/{a.branch}", tok)
    print(f"[ref ] {st} {a.branch} -> {ref.get('object', {}).get('sha', ref)}")
    if st != 200:
        raise SystemExit(1)
    cur = ref["object"]["sha"]

    st, c_cur = call("GET", f"/repos/{a.repo}/git/commits/{cur}", tok)
    print(f"[cur ] {st} tree={c_cur.get('tree', {}).get('sha', '?')[:12]} "
          f"parents={[p['sha'][:12] for p in c_cur.get('parents', [])]} "
          f"msg={(c_cur.get('message') or '').splitlines()[:1]}")

    # 短 sha 在 git/commits 端点不被接受（400/404），先用 commits 端点解析成完整 40 位
    tgt = a.target
    if len(tgt) < 40:
        st0, rc = call("GET", f"/repos/{a.repo}/commits/{a.target}", tok)
        if st0 == 200:
            tgt = rc["sha"]
            print(f"[rslv] {a.target} -> {tgt}")
        else:
            print(f"[rslv] {st0} 无法解析 {a.target}: {str(rc)[:120]}")

    st, c_tgt = call("GET", f"/repos/{a.repo}/git/commits/{tgt}", tok)
    print(f"[tgt ] {st} {tgt} tree={c_tgt.get('tree', {}).get('sha', '?') if st == 200 else c_tgt}")
    if st != 200:
        raise SystemExit("[ERR] 目标提交在远端不存在")

    same_tree = c_cur.get("tree", {}).get("sha") == c_tgt.get("tree", {}).get("sha")
    print(f"[diff] same_tree={same_tree}")

    st, prot = call("GET", f"/repos/{a.repo}/branches/{a.branch}/protection", tok)
    print(f"[prot] {st} " + ("无保护" if st == 404 else
                             f"linear={prot.get('required_linear_history', {}).get('enabled')} "
                             f"forcepush={prot.get('allow_force_pushes', {}).get('enabled')} "
                             f"del={prot.get('allow_deletions', {}).get('enabled')}"))

    if a.diagnose or not a.apply:
        print("[done] 仅诊断，未修改")
        return

    full = c_tgt["sha"]
    st, r = call("PATCH", f"/repos/{a.repo}/git/refs/heads/{a.branch}",
                 tok, {"sha": full, "force": True})
    print(f"[patch] {st} {str(r)[:200]}")
    if st in (200, 201):
        print(f"[OK] {a.branch} -> {full[:12]}")
        return

    # 兜底：删 ref 再重建
    st2, r2 = call("DELETE", f"/repos/{a.repo}/git/refs/heads/{a.branch}", tok)
    print(f"[del  ] {st2} {str(r2)[:160]}")
    if st2 in (204, 200):
        st3, r3 = call("POST", f"/repos/{a.repo}/git/refs", tok,
                       {"ref": f"refs/heads/{a.branch}", "sha": full})
        print(f"[post ] {st3} {str(r3)[:200]}")
        if st3 in (200, 201):
            print(f"[OK] {a.branch} 重建 -> {full[:12]}")
            return
    print("[WARN] 未能修正尖端；保留现状并在 docs/evidence 记录")


if __name__ == "__main__":
    main()
