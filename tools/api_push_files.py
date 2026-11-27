#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""api_push_files.py —— 走 GitHub REST API 把一批文件变更推成分支上的一个提交。

用途：当 `github.com:443` 被墙 / 代理不可用、但 `api.github.com:443` 可达时，
git 协议推不上去，可改用 Git Data API（blobs → tree → commit → ref）完成同步。

不改变工作区，不依赖 git remote，只认「源仓库某两个 commit 之间的文件差异」。

用法：
    # 把 work 仓库 96c5922..28b872d 的变更，推到 GitHub 的 feat/p3-native-build
    python tools/api_push_files.py \
        --src-repo /e/源端/源/zhengtugf-work \
        --since 96c5922 --until 28b872d \
        --repo taoxulove1/zhengtgf --branch feat/p3-native-build \
        --message "docs: 记录登录器与客户端启动验证"

    # 只看会推什么，不真的提交
    python tools/api_push_files.py ... --dry-run
"""

import argparse
import base64
import json
import os
import subprocess
import sys
import urllib.error
import urllib.request

API = "https://api.github.com"


def git(repo, *args):
    return subprocess.run(["git", "-C", repo, *args], capture_output=True, text=True, check=True).stdout


def token():
    t = os.environ.get("GITHUB_TOKEN", "")
    if t:
        return t
    try:
        out = subprocess.run(
            ["git", "credential", "fill"],
            input="protocol=https\nhost=github.com\n\n",
            capture_output=True, text=True, check=True).stdout
        for line in out.splitlines():
            if line.startswith("password="):
                return line.split("=", 1)[1].strip()
    except Exception:
        pass
    raise SystemExit("[ERR] 需要 GITHUB_TOKEN 或 git credential 里的口令")


def call(method, path, tok, body=None, allow=(200, 201)):
    url = API + path
    data = json.dumps(body).encode() if body is not None else None
    req = urllib.request.Request(url, data=data, method=method)
    req.add_header("Authorization", "token " + tok)
    req.add_header("Accept", "application/vnd.github+json")
    req.add_header("Content-Type", "application/json")
    try:
        with urllib.request.urlopen(req, timeout=30) as r:
            return json.loads(r.read().decode())
    except urllib.error.HTTPError as e:
        detail = e.read().decode("utf-8", "replace")[:400]
        raise SystemExit(f"[ERR] {method} {path} -> {e.code}: {detail}")


def call_soft(method, path, tok, body=None):
    """不抛异常的调用，返回 (status, json)。用于「分支可能不存在」这类预期失败。"""
    try:
        return 200, call(method, path, tok, body)
    except SystemExit as e:
        msg = str(e)
        code = 0
        for part in msg.split():
            if part.isdigit():
                code = int(part)
                break
        return code, {"detail": msg[:300]}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--src-repo", required=True)
    ap.add_argument("--since", required=True)
    ap.add_argument("--until", required=True)
    ap.add_argument("--repo", required=True, help="owner/name")
    ap.add_argument("--branch", required=True)
    ap.add_argument("--message", required=True)
    ap.add_argument("--dry-run", action="store_true")
    ap.add_argument("--base", default="main", help="分支不存在时从哪个分支拉出来")
    a = ap.parse_args()

    files = [f for f in git(a.src_repo, "diff", "--name-only", f"{a.since}..{a.until}").splitlines() if f]
    if not files:
        print("[i] 两个 commit 之间没有差异")
        return 0
    print(f"待推 {len(files)} 个文件：")
    payload = []
    for f in files:
        raw = subprocess.run(["git", "-C", a.src_repo, "show", f"{a.until}:{f}"],
                             capture_output=True, check=True).stdout
        print(f"  {f:<44} {len(raw):>7} B")
        payload.append((f, raw))
    if a.dry_run:
        print("[dry-run] 未提交")
        return 0

    tok = token()
    st, ref = call_soft("GET", f"/repos/{a.repo}/git/ref/heads/{a.branch}", tok)
    if st != 200:
        stb, bref = call_soft("GET", f"/repos/{a.repo}/git/ref/heads/{a.base}", tok)
        if stb != 200:
            raise SystemExit(f"[ERR] 基线分支 {a.base} 也读不到: {stb}")
        bsha = bref["object"]["sha"]
        stc, _ = call_soft("POST", f"/repos/{a.repo}/git/refs", tok,
                           {"ref": f"refs/heads/{a.branch}", "sha": bsha})
        print(f"[new] 远端无 {a.branch}，已从 {a.base}@{bsha[:7]} 新建 (HTTP {stc})")
        st, ref = call_soft("GET", f"/repos/{a.repo}/git/ref/heads/{a.branch}", tok)
        if st != 200:
            raise SystemExit(f"[ERR] 新建分支后仍读不到: {st} {ref}")
    base = ref["object"]["sha"]
    base_commit = call("GET", f"/repos/{a.repo}/git/commits/{base}", tok)
    base_tree = base_commit["tree"]["sha"]
    print(f"基线 {a.branch} = {base[:7]}  tree={base_tree[:7]}")

    tree_items = []
    for f, raw in payload:
        blob = call("POST", f"/repos/{a.repo}/git/blobs", tok,
                    {"content": base64.b64encode(raw).decode(), "encoding": "base64"})
        tree_items.append({"path": f, "mode": "100644", "type": "blob", "sha": blob["sha"]})
        print(f"  blob {f} -> {blob['sha'][:7]}")

    tree = call("POST", f"/repos/{a.repo}/git/trees", tok,
                {"base_tree": base_tree, "tree": tree_items})
    commit = call("POST", f"/repos/{a.repo}/git/commits", tok,
                  {"message": a.message, "tree": tree["sha"], "parents": [base]})
    call("PATCH", f"/repos/{a.repo}/git/refs/heads/{a.branch}", tok,
         {"sha": commit["sha"], "force": False})
    print(f"[OK] {a.branch} {base[:7]} -> {commit['sha'][:7]}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
