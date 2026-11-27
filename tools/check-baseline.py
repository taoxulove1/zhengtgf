#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""check-baseline.py — 客户端污染哨兵。

两个目录必须字节级同源：
    工作副本  --work  E:/源端/官方      （会启动、会抓包、可能被 patchupdate 改写）
    只读基线  --base  E:/源端/original  （解压后从未运行，唯一可信参照）

判据：两棵树按 path 排序后逐行 path:sha256 拼接取 MD5，值相等即同源。
任何一次客户端启动后都该跑一遍；不等就说明被改了，立刻回滚。

用法:
    python tools/check-baseline.py
    python tools/check-baseline.py --work "E:/源端/官方" --base "E:/源端/original"
    python tools/check-baseline.py --detail          # 打印差异路径（最多 40 条）

退出码: 0 同源 / 1 有差异 / 2 目录缺失
"""

import argparse
import hashlib
import os
import sys

EXPECT = "eea3a291dc42150e7057179141e10b78"
CHUNK = 8 << 20


def sha256_of(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for b in iter(lambda: f.read(CHUNK), b""):
            h.update(b)
    return h.hexdigest()


def snapshot(root, skip=(".dl", ".tmp", ".part", "_temp")):
    files = {}
    for dp, dn, fn in os.walk(root):
        for name in fn:
            if name.lower().endswith(skip):
                continue
            p = os.path.join(dp, name)
            rel = os.path.relpath(p, root).replace("\\", "/")
            try:
                files[rel] = (os.path.getsize(p), sha256_of(p))
            except OSError as e:
                print(f"  [SKIP] {rel}: {e}", file=sys.stderr)
    return files


def fingerprint(files):
    m = hashlib.md5()
    for k in sorted(files):
        m.update(f"{k}:{files[k][1]}\n".encode())
    return m.hexdigest()


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--work", default="E:/源端/官方")
    ap.add_argument("--base", default="E:/源端/original")
    ap.add_argument("--expect", default=EXPECT, help="期望指纹；传空字符串则跳过")
    ap.add_argument("--detail", action="store_true")
    a = ap.parse_args()

    for d in (a.work, a.base):
        if not os.path.isdir(d):
            print(f"[ERR] 目录不存在: {d}", file=sys.stderr)
            return 2

    w = snapshot(a.work)
    b = snapshot(a.base)
    fw, fb = fingerprint(w), fingerprint(b)

    print(f"工作副本 {a.work}\n  {len(w)} 文件  {sum(v[0] for v in w.values()) / 1073741824:.2f} GiB  md5={fw}")
    print(f"只读基线 {a.base}\n  {len(b)} 文件  {sum(v[0] for v in b.values()) / 1073741824:.2f} GiB  md5={fb}")

    ok = True
    if fw != fb:
        ok = False
        print("\n[不一致] 工作副本已被改写")
    else:
        print("\n[同源] 两棵树字节级一致")

    if a.expect and fw != a.expect:
        ok = False
        print(f"[偏离基线] 期望 {a.expect}，实际 {fw}")

    if a.detail and not ok:
        add = sorted(set(w) - set(b))
        rm = sorted(set(b) - set(w))
        chg = sorted(k for k in set(w) & set(b) if w[k] != b[k])
        print(f"\n新增 {len(add)} / 缺失 {len(rm)} / 内容不同 {len(chg)}")
        for tag, items in (("+", add), ("-", rm), ("~", chg)):
            for k in items[:40]:
                print(f"  {tag} {k}")

    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
