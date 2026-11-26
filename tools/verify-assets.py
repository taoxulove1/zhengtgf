#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""verify-assets.py — 按 assets/manifest/assets.json 校验实物完整性。

用法:
    python tools/verify-assets.py --root "E:/源端/官方"                # 全量校验
    python tools/verify-assets.py --root "E:/源端/官方" --sample 50    # 抽样 50 个
    python tools/verify-assets.py --root . --missing-only              # 只报缺失

退出码: 0 全通过 / 1 有差异 / 2 参数或清单错误
"""

import argparse
import hashlib
import json
import os
import sys

CHUNK = 8 * 1024 * 1024


def sha256_of(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        while True:
            b = f.read(CHUNK)
            if not b:
                break
            h.update(b)
    return h.hexdigest()


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--manifest", default="assets/manifest/assets.json")
    ap.add_argument("--root", required=True, help="实物根目录；archive 条目按 basename 在 root 下找")
    ap.add_argument("--sample", type=int, default=0, help="抽样条数，0 = 全量")
    ap.add_argument("--missing-only", action="store_true")
    ap.add_argument("--strict-size", action="store_true", help="仅比体积不重算哈希（快）")
    args = ap.parse_args()

    if not os.path.exists(args.manifest):
        print(f"[ERR] 清单不存在: {args.manifest}", file=sys.stderr)
        return 2
    with open(args.manifest, encoding="utf-8") as f:
        doc = json.load(f)

    files = doc.get("files", [])
    if args.sample and args.sample < len(files):
        step = max(1, len(files) // args.sample)
        files = files[::step][: args.sample]

    missing, size_bad, hash_bad, ok = [], [], [], 0
    for r in files:
        p = os.path.join(args.root, r["path"].replace("/", os.sep))
        if not os.path.exists(p):
            alt = os.path.join(args.root, os.path.basename(r["path"]))
            if os.path.exists(alt):
                p = alt
            else:
                missing.append(r["path"])
                continue
        real = os.path.getsize(p)
        if real != r["size"]:
            size_bad.append((r["path"], r["size"], real))
            continue
        if args.strict_size:
            ok += 1
            continue
        if sha256_of(p) != r["sha256"]:
            hash_bad.append(r["path"])
            continue
        ok += 1

    print(f"清单: {args.manifest}")
    print(f"校验: {len(files)} 条  OK={ok}  缺失={len(missing)}  体积不符={len(size_bad)}  哈希不符={len(hash_bad)}")
    if not args.missing_only:
        for m in missing[:20]:
            print(f"  [MISS] {m}")
        for p, e, a in size_bad[:20]:
            print(f"  [SIZE] {p}  期望 {e}  实际 {a}")
        for p in hash_bad[:20]:
            print(f"  [HASH] {p}")
    else:
        for m in missing[:50]:
            print(f"  [MISS] {m}")

    return 0 if not (missing or size_bad or hash_bad) else 1


if __name__ == "__main__":
    sys.exit(main())
