# -*- coding: utf-8 -*-
"""定位 patchupdate.exe 中 .co 文件的处理链：
"%s.co" 字符串 xref -> 下载完成回调 -> 解包/解密函数 -> 密钥材料。"""
import pefile
from capstone import Cs, CS_ARCH_X86, CS_MODE_32

path = r"E:\源端\源1\patchupdate.exe"
pe = pefile.PE(path)
base = pe.OPTIONAL_HEADER.ImageBase

secs = []
for s in pe.sections:
    secs.append((s.Name.decode().rstrip("\0"), base + s.VirtualAddress, s.get_data()))

code_va, code = [(v, d) for n, v, d in secs if n == ".text"][0]


def find_all(needle):
    hits = []
    for n, v, d in secs:
        i = 0
        while True:
            i = d.find(needle, i)
            if i < 0:
                break
            hits.append(v + i)
            i += 1
    return hits


needles = [b"%s.co", b".co\x00", b".dl\x00", b"%s.dl",
           b"XPAK", b"xpak", b"GZIP", b"ZLIB",
           b"Decrypt", b"Encrypt", b"des", b"Des", b"DES",
           b"ProcessPack", b"PackFile", b"unpack", b"Unpack",
           b"inflate", "zlib".encode(), b"uncompress", b"compress",
           b"\\autopatch", b"versionsign"]
addrs = {}
for nd in needles:
    addrs[nd] = find_all(nd)
    print(nd, "->", [hex(a) for a in addrs[nd][:8]], "(%d)" % len(addrs[nd]))

vas = {}
for nd, lst in addrs.items():
    for a in lst:
        vas[a] = nd

refs = {}
for off in range(len(code) - 5):
    if code[off] == 0x68:  # push imm32
        imm = int.from_bytes(code[off + 1:off + 5], "little")
        if imm in vas:
            refs.setdefault(imm, []).append(("push", off))
    if 0xB8 <= code[off] <= 0xBF:  # mov r32, imm32
        imm = int.from_bytes(code[off + 1:off + 5], "little")
        if imm in vas:
            refs.setdefault(imm, []).append(("mov", off))

md = Cs(CS_ARCH_X86, CS_MODE_32)
for va, lst in sorted(refs.items()):
    nd = vas[va]
    for kind, off in lst[:4]:
        start = max(0, off - 100)
        print("=" * 70)
        print(nd.decode(errors="replace"), hex(va), kind, "at", hex(code_va + off))
        for ins in md.disasm(code[start:off + 8], code_va + start):
            print("   %s %s" % (ins.mnemonic, ins.op_str))
