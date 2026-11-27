#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""gh_branches.py —— 列出远端分支尖端 + 与给定基线的关系（只读）。"""
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
    out = subprocess.run(["git", "credential", "fill"],
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
        return e.code, {"raw": e.read().decode()[:200]}


def main():
    repo = sys.argv[1] if len(sys.argv) > 1 else "taoxulove1/zhengtgf"
    tok = token()
    st, brs = call("GET", f"/repos/{repo}/branches?per_page=100", tok)
    if st != 200:
        raise SystemExit(f"[ERR] {st} {brs}")
    print(f"{'分支':38s} {'尖端':12s} {'落后/领先 main':16s} 最后提交时间")
    tips = {}
    for b in brs:
        n = b["name"]
        sha = b["commit"]["sha"]
        tips[n] = sha
        print(f"{n:38s} {sha[:12]}")
    # 每个分支相对 main 的 ahead/behind
    main_sha = tips.get("main")
    if main_sha:
        for n, sha in tips.items():
            if n == "main":
                continue
            st2, cmp = call("GET", f"/repos/{repo}/compare/{main_sha}...{sha}", tok)
            if st2 == 200:
                print(f"[cmp] {n:32s} ahead={cmp['ahead_by']:<3d} behind={cmp['behind_by']:<3d} "
                      f"files={len(cmp.get('files', []))}")
    st3, prs = call("GET", f"/repos/{repo}/pulls?state=open&per_page=50", tok)
    if st3 == 200:
        print(f"[pr ] open={len(prs)} " + ", ".join(f"#{p['number']}:{p['head']['ref']}" for p in prs))


if __name__ == "__main__":
    main()
