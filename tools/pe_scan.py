#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""pe_scan.py —— 极简 PE 头部分析（只读前 4KB + 节表），判断是否加壳 / 保护 / 反调试特征。

用法：python3 pe_scan.py <exe> [<exe> ...]
输出：机器字长、入口点节、各节名称/大小/熵、导入表 DLL 列表、可疑特征字符串。
"""
import math
import struct
import sys


def entropy(b):
    if not b:
        return 0.0
    from collections import Counter
    c = Counter(b)
    n = float(len(b))
    return -sum((v / n) * math.log(v / n, 2) for v in c.values())


def scan(path):
    d = open(path, "rb").read()
    print("=" * 62)
    print(f"{path}  size={len(d)}  entropy={entropy(d[:0x100000]):.2f}")
    if d[:2] != b"MZ":
        print("  非 MZ")
        return
    e_lfanew = struct.unpack_from("<I", d, 0x3C)[0]
    if d[e_lfanew:e_lfanew + 4] != b"PE\0\0":
        print("  非 PE")
        return
    coff = e_lfanew + 4
    machine, nsec = struct.unpack_from("<HH", d, coff)
    opt_size = struct.unpack_from("<H", d, coff + 16)[0]
    chars = struct.unpack_from("<H", d, coff + 18)[0]
    opt = coff + 20
    magic = struct.unpack_from("<H", d, opt)[0]
    pe32p = magic == 0x20B
    print(f"  machine=0x{machine:04X} sections={nsec} "
          f"{'PE32+' if pe32p else 'PE32'} chars=0x{chars:04X} "
          f"(relocs_off={bool(chars & 0x0001)} exe={bool(chars & 0x0002)} dll={bool(chars & 0x2000)})")
    ep = struct.unpack_from("<I", d, opt + 16)[0]
    imgbase = struct.unpack_from("<I", d, opt + (24 if pe32p else 28))[0]
    print(f"  imagebase=0x{imgbase:08X} entry=0x{ep:08X} (rva)")
    ddir_off = opt + (112 if pe32p else 96)
    nrva = struct.unpack_from("<I", d, ddir_off - 4)[0]
    dirs = []
    for i in range(min(nrva, 16)):
        rva, sz = struct.unpack_from("<II", d, ddir_off + i * 8)
        dirs.append((rva, sz))
    sec_off = opt + opt_size
    secs = []
    for i in range(nsec):
        o = sec_off + i * 40
        name = d[o:o + 8].rstrip(b"\0").decode("latin1")
        vsize, vaddr, rawsz, rawptr = struct.unpack_from("<IIII", d, o + 8)
        sflags = struct.unpack_from("<I", d, o + 36)[0]
        body = d[rawptr:rawptr + min(rawsz, 0x200000)] if rawptr else b""
        secs.append((name, vaddr, vsize, rawsz, rawptr, sflags))
        print(f"  sec {name:9s} va=0x{vaddr:08X} vs=0x{vsize:08X} raw=0x{rawsz:08X} "
              f"flags=0x{sflags:08X} H={entropy(body):.2f}")
    # 入口点落在哪个节
    for name, vaddr, vsize, rawsz, rawptr, _ in secs:
        if vaddr <= ep < vaddr + max(vsize, rawsz):
            print(f"  entry 位于节 {name}")
    # 导入表
    imp_rva, imp_sz = dirs[1]
    names = []
    for name, vaddr, vsize, rawsz, rawptr, _ in secs:
        if rawptr and vaddr <= imp_rva < vaddr + rawsz:
            off = rawptr + (imp_rva - vaddr)
            while off + 20 <= len(d):
                ilt, ts, fc, nmrva, iat = struct.unpack_from("<IIIII", d, off)
                if nmrva == 0 and ilt == 0 and iat == 0:
                    break
                for s2 in secs:
                    if s2[4] and s2[1] <= nmrva < s2[1] + s2[3]:
                        p = s2[4] + (nmrva - s2[1])
                        end = d.find(b"\0", p)
                        names.append(d[p:end].decode("latin1", "replace"))
                        break
                off += 20
            break
    print(f"  导入 DLL ({len(names)}): {', '.join(names)}")
    # 特征串
    low = d.lower()
    for kw in (b"debugger", b"themida", b"winlicense", b"vmp", b"vmprotect", b"enigma",
               b"aspack", b"upx", b"hprotect", b"nprotect", b"ztsafe", b"dbg", b"isdebuggerpresent"):
        c = low.count(kw)
        if c:
            print(f"  特征 {kw.decode():18s} x{c}")


if __name__ == "__main__":
    for p in sys.argv[1:]:
        try:
            scan(p)
        except Exception as e:
            print(f"{p}: {e}")
