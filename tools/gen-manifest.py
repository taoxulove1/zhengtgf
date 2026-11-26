#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""gen-manifest.py — 扫描源目录，生成资产清单（CSV / JSON / sha256sum）。

用法:
    python tools/gen-manifest.py --source "E:/源端/官方" --out assets/manifest
    python tools/gen-manifest.py --source "E:/源端/官方" --out assets/manifest \
        --extra "E:/源端/VS2022服务端server.rar" --extra "E:/源端/客服端源码.rar"

字段: path,size,sha256,group,ext,route
route 分层规则（见 docs/assets-policy.md）：
    <1MB            git
    1MB  – 10MB     git-optional
    10MB – 100MB    lfs-or-release
    >=100MB         release-split   （GitHub 单文件硬限 100MB，必须切分）
"""

import argparse
import csv
import hashlib
import json
import os
import sys
from datetime import datetime, timezone

CHUNK = 8 * 1024 * 1024
MB = 1024 * 1024


def route_of(size: int) -> str:
    if size >= 100 * MB:
        return "release-split"
    if size >= 10 * MB:
        return "lfs-or-release"
    if size >= 1 * MB:
        return "git-optional"
    return "git"


def sha256_of(path: str) -> str:
    h = hashlib.sha256()
    with open(path, "rb") as f:
        while True:
            block = f.read(CHUNK)
            if not block:
                break
            h.update(block)
    return h.hexdigest()


def scan(roots):
    rows = []
    total = 0
    for root in roots:
        root = os.path.abspath(root)
        base_name = os.path.basename(root.rstrip("\\/"))
        if os.path.isfile(root):
            size = os.path.getsize(root)
            rows.append({
                "path": base_name,
                "size": size,
                "sha256": sha256_of(root),
                "group": "archive",
                "ext": os.path.splitext(root)[1].lower().lstrip(".") or "(none)",
                "route": route_of(size),
                "mtime": datetime.fromtimestamp(os.path.getmtime(root), timezone.utc)
                           .strftime("%Y-%m-%dT%H:%M:%SZ"),
            })
            total += size
            print(f"  + {base_name}  {size / MB:.1f} MB", flush=True)
            continue
        for dirpath, dirnames, filenames in os.walk(root):
            dirnames.sort()
            for name in sorted(filenames):
                full = os.path.join(dirpath, name)
                try:
                    size = os.path.getsize(full)
                except OSError:
                    continue
                rel = os.path.relpath(full, root).replace("\\", "/")
                ext = os.path.splitext(name)[1].lower().lstrip(".") or "(none)"
                rows.append({
                    "path": f"{base_name}/{rel}",
                    "size": size,
                    "sha256": sha256_of(full),
                    "group": rel.split("/")[0] if "/" in rel else "(root)",
                    "ext": ext,
                    "route": route_of(size),
                    "mtime": datetime.fromtimestamp(os.path.getmtime(full), timezone.utc)
                               .strftime("%Y-%m-%dT%H:%M:%SZ"),
                })
                total += size
    rows.sort(key=lambda r: r["path"].lower())
    return rows, total


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--source", action="append", required=True,
                    help="源目录或文件，可重复")
    ap.add_argument("--extra", action="append", default=[], help="附加大文件，可重复")
    ap.add_argument("--out", default="assets/manifest")
    ap.add_argument("--no-sha", action="store_true", help="跳过 sha256（仅统计用）")
    args = ap.parse_args()

    roots = list(args.source) + list(args.extra)
    os.makedirs(args.out, exist_ok=True)

    print(f"[1/3] 扫描 {len(roots)} 个根 ...", flush=True)
    rows, total = scan(roots)
    print(f"[2/3] 共 {len(rows)} 个文件，合计 {total / MB:.1f} MB", flush=True)

    csv_path = os.path.join(args.out, "assets.csv")
    with open(csv_path, "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=["path", "size", "sha256", "group", "ext", "route", "mtime"])
        w.writeheader()
        for r in rows:
            w.writerow(r)

    json_path = os.path.join(args.out, "assets.json")
    summary = {}
    for r in rows:
        k = r["route"]
        s = summary.setdefault(k, {"count": 0, "bytes": 0})
        s["count"] += 1
        s["bytes"] += r["size"]
    doc = {
        "generated_at": datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ"),
        "source_roots": roots,
        "file_count": len(rows),
        "total_bytes": total,
        "route_summary": summary,
        "files": rows,
    }
    with open(json_path, "w", encoding="utf-8") as f:
        json.dump(doc, f, ensure_ascii=False, indent=2)

    sha_path = os.path.join(args.out, "checksums.sha256")
    with open(sha_path, "w", encoding="utf-8") as f:
        for r in rows:
            f.write(f"{r['sha256']}  {r['path']}\n")

    print(f"[3/3] 写出:\n  {csv_path}\n  {json_path}\n  {sha_path}")
    for k, v in sorted(summary.items()):
        print(f"  route={k:<16} {v['count']:>4} 个  {v['bytes'] / MB:>9.1f} MB")


if __name__ == "__main__":
    sys.exit(main())
