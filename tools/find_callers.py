# -*- coding: utf-8 -*-
"""全量调用点扫描：MD5 / 密钥派生 / 解密函数 / pack-open 的所有 caller。"""
import pefile
import struct
from capstone import Cs, CS_ARCH_X86, CS_MODE_32

path = r"E:\源端\源1\patchupdate.exe"
pe = pefile.PE(path)
base = pe.OPTIONAL_HEADER.ImageBase
secs = [(s.Name.decode().rstrip("\0"), base + s.VirtualAddress, s.get_data()) for s in pe.sections]
code_va, code = [(v, d) for n, v, d in secs if n == ".text"][0]
md = Cs(CS_ARCH_X86, CS_MODE_32); md.skipdata = True

TARGETS = {
    0x44ee80: "MD5String",
    0x44ee20: "MD5Data",
    0x4291c0: "KeyDerive",
    0x429040: "DecryptHDRtbl",
    0x429100: "EncryptHDRtbl",
    0x44ed00: "BlockA",
    0x44ebe0: "BlockB",
    0x44e140: "DES_set_key",
}

# 扫描 E8 rel32 调用
callers = {t: [] for t in TARGETS}
for off in range(len(code) - 5):
    if code[off] == 0xE8:
        rel = struct.unpack("<i", code[off + 1:off + 5])[0]
        tgt = code_va + off + 5 + rel
        if tgt in callers:
            callers[tgt].append(code_va + off)

for t, lst in callers.items():
    print("%s %s: %d callers" % (TARGETS[t], hex(t), len(lst)))
    for c in lst[:20]:
        print("    <-", hex(c))

# 对每个 caller 反汇编前 120 字节看上下文
def ctx(addr, back=100, fwd=60):
    start = addr - code_va - back
    print("=" * 70, "context of", hex(addr))
    for ins in md.disasm(code[start:start + back + fwd], code_va + start):
        print("   %08x %-8s %s" % (ins.address, ins.mnemonic, ins.op_str))

print("\n### KeyDerive callers context:")
for c in callers[0x4291c0][:3]:
    ctx(c)
