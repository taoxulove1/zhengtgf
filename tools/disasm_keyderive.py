# -*- coding: utf-8 -*-
"""反汇编 0x4291c0（疑似 PasswordToDesKey）与 0x429040（疑似 DecryptHeaderData），
挖出密码字符串引用与 MD5 常量。"""
import pefile
from capstone import Cs, CS_ARCH_X86, CS_MODE_32

path = r"E:\源端\源1\patchupdate.exe"
pe = pefile.PE(path)
base = pe.OPTIONAL_HEADER.ImageBase
secs = [(s.Name.decode().rstrip("\0"), base + s.VirtualAddress, s.get_data()) for s in pe.sections]
code_va, code = [(v, d) for n, v, d in secs if n == ".text"][0]
rodata = dict((v, d) for n, v, d in secs if n == ".rdata")

md = Cs(CS_ARCH_X86, CS_MODE_32)
md.skipdata = True

def dump(va, length=0x200, label=""):
    print("#" * 70, label, hex(va))
    off = va - code_va
    for ins in md.disasm(code[off:off + length], va):
        s = "%08x %-8s %s" % (ins.address, ins.mnemonic, ins.op_str)
        # 解析立即数指向 .rdata 的字符串
        for tok in ins.op_str.split():
            if tok.startswith("0x"):
                try:
                    v = int(tok, 16)
                except ValueError:
                    continue
                if v in rodata:
                    d = rodata[v][:48]
                    txt = d.split(b"\x00")[0]
                    try:
                        s += "   ; \"%s\"" % txt.decode("gbk", "replace")
                    except Exception:
                        s += "   ; %r" % txt
        print(s)

dump(0x4291c0, 0x180, "KeyDerive?")
dump(0x429040, 0x180, "DecryptHeaderData?")
