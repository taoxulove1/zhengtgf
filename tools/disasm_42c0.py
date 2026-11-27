# -*- coding: utf-8 -*-
"""完整反汇编 0x42c0xx 函数（XPAK 写头处）+ 找 MD5 常量 xref 定位 PasswordToDesKey。"""
import pefile
from capstone import Cs, CS_ARCH_X86, CS_MODE_32

path = r"E:\源端\源1\patchupdate.exe"
pe = pefile.PE(path)
base = pe.OPTIONAL_HEADER.ImageBase
secs = [(s.Name.decode().rstrip("\0"), base + s.VirtualAddress, s.get_data()) for s in pe.sections]
code_va, code = [(v, d) for n, v, d in secs if n == ".text"][0]

md = Cs(CS_ARCH_X86, CS_MODE_32)
md.skipdata = True

# 1) 找函数起点：从 0x42c149 向前扫 push ebp / mov ebp,esp 或 int3 对齐
START = 0x42c000
off0 = START - code_va
# 线性反汇编 0x42c000..0x42d000
print("### linear disasm 0x42c000-0x42d000")
for ins in md.disasm(code[off0:off0 + 0x1000], START):
    mark = ""
    if ins.address in (0x42c149, 0x42c215, 0x42c293):
        mark = "   <== XPAK push"
    print("%08x %-8s %s%s" % (ins.address, ins.mnemonic, ins.op_str, mark))
