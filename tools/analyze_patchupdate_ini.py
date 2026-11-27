# -*- coding: utf-8 -*-
"""静态分析 patchupdate.exe 的 GetPrivateProfile* 调用点，还原 ini 键名与读取顺序。"""
import struct
import pefile
from capstone import Cs, CS_ARCH_X86, CS_MODE_32

path = r"E:\源端\源1\patchupdate.exe"
pe = pefile.PE(path)
base = pe.OPTIONAL_HEADER.ImageBase

targets = {"GetPrivateProfileStringA", "GetPrivateProfileStringW",
           "GetPrivateProfileIntA", "GetPrivateProfileIntW",
           "GetPrivateProfileSectionNamesA", "GetPrivateProfileSectionA"}
thunks = {}
for entry in pe.DIRECTORY_ENTRY_IMPORT:
    for imp in entry.imports:
        if imp.name and imp.name.decode() in targets:
            thunks[imp.name.decode()] = imp.address
print("IAT thunks:", {k: hex(v) for k, v in thunks.items()})

secs = []
for s in pe.sections:
    n = s.Name.decode().rstrip("\0")
    secs.append((n, base + s.VirtualAddress, s.get_data()))

code_va, code = [(v, d) for n, v, d in secs if n == ".text"][0]

call_sites = []
for off in range(len(code) - 6):
    if code[off] == 0xFF and code[off + 1] == 0x15:
        addr = struct.unpack("<I", code[off + 2:off + 6])[0]
        for k, v in thunks.items():
            if v == addr:
                call_sites.append((off, k))
print("call sites:", len(call_sites))


def get_str(va):
    for n, v, d in secs:
        if v <= va < v + len(d):
            z = d[va - v:].split(b"\0")[0]
            return z
    return None


md = Cs(CS_ARCH_X86, CS_MODE_32)
for off, fname in call_sites:
    start = max(0, off - 200)
    pushes = []
    for ins in md.disasm(code[start:off + 6], code_va + start):
        if ins.mnemonic == "push" and ins.op_str.startswith("0x"):
            try:
                va = int(ins.op_str, 16)
                s = get_str(va)
                if s and 1 < len(s) < 80:
                    try:
                        pushes.append(s.decode("gbk"))
                    except UnicodeDecodeError:
                        pushes.append(s.decode("latin1"))
            except ValueError:
                pass
    print("=" * 60)
    print(hex(code_va + off), fname)
    for p in pushes[-10:]:
        print("   |", p)
