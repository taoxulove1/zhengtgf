# -*- coding: utf-8 -*-
"""定位 patchupdate.exe 关键字符串的交叉引用，还原 ini/注册表读取逻辑。"""
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


needles = [b"patchsetup.ini", b"zt_client_autoupath", b"InstallPath",
           b"update\x00", b"Update\x00", b"UPDATE\x00",
           b"GetPrivateProfileString", b"srvlistall", b"servername\x00",
           b"showstartui", b"PatchType"]
addrs = {}
for nd in needles:
    addrs[nd] = find_all(nd)
    print(nd, "->", [hex(a) for a in addrs[nd]])

# 扫 .text 中引用这些 VA 的立即数 (push imm32 = 68 xx, mov reg, imm32 = B8+r / C7)
vas = {}
for nd, lst in addrs.items():
    for a in lst:
        vas[a] = nd

refs = {}
for off in range(len(code) - 5):
    # push imm32
    if code[off] == 0x68:
        imm = int.from_bytes(code[off + 1:off + 5], "little")
        if imm in vas:
            refs.setdefault(imm, []).append(("push", off))
    # mov r32, imm32 (B8+r)
    if 0xB8 <= code[off] <= 0xBF:
        imm = int.from_bytes(code[off + 1:off + 5], "little")
        if imm in vas:
            refs.setdefault(imm, []).append(("mov", off))
    # C7 /2 mov [mem], imm32 略——push/mov 足够

md = Cs(CS_ARCH_X86, CS_MODE_32)
for va, lst in sorted(refs.items()):
    nd = vas[va]
    for kind, off in lst[:6]:
        start = max(0, off - 90)
        print("=" * 70)
        print(nd.decode(), hex(va), kind, "at", hex(code_va + off))
        for ins in md.disasm(code[start:off + 8], code_va + start):
            print("   %s %s" % (ins.mnemonic, ins.op_str))
