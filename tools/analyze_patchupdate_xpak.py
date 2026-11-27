# -*- coding: utf-8 -*-
"""XPAK 魔数 xref + 完整反汇编所在函数：patchupdate 的 pack 校验/解密入口。"""
import pefile
from capstone import Cs, CS_ARCH_X86, CS_MODE_32

path = r"E:\源端\源1\patchupdate.exe"
pe = pefile.PE(path)
base = pe.OPTIONAL_HEADER.ImageBase

secs = []
for s in pe.sections:
    secs.append((s.Name.decode().rstrip("\0"), base + s.VirtualAddress, s.get_data()))
code_va, code = [(v, d) for n, v, d in secs if n == ".text"][0]
rdata = dict((v, d) for n, v, d in secs if n in (".rdata", ".data"))

TARGETS = [0x46db24]  # XPAK
refs = []
for off in range(len(code) - 5):
    if code[off] == 0x68:
        imm = int.from_bytes(code[off + 1:off + 5], "little")
        if imm in TARGETS:
            refs.append(("push", off))
    if 0xB8 <= code[off] <= 0xBF:
        imm = int.from_bytes(code[off + 1:off + 5], "little")
        if imm in TARGETS:
            refs.append(("mov", off))

md = Cs(CS_ARCH_X86, CS_MODE_32)
for kind, off in refs:
    print("=" * 70, kind, "at", hex(code_va + off))
    start = max(0, off - 60)
    for ins in md.disasm(code[start:off + 80], code_va + start):
        print("   %08x %s %s" % (ins.address, ins.mnemonic, ins.op_str))
