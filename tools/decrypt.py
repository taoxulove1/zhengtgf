#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""decrypt.py — 征途 3.1.46.0 报文解密与结构化。

算法来源（实测源码，非推测）：
    src/server/base/CDes.cpp  CDes::encdec()
        while (offset <= nLen-8) { DES_ecb_encrypt(data+offset, ...); offset += 8; }
        return nLen - offset;
    → 裸 DES-ECB，每 8 字节独立一块，无 IV、无链接、无填充，尾部不足 8 字节保持明文。

包结构（src/server/base/Command.h stNullUserCmd）：
    +0  BYTE byCmd          命令域，如 LOGON_USERCMD
    +1  BYTE byParam        子命令
    +2  DWORD dwTimestamp   4 字节
    +6  payload             → opcode = (byCmd << 8) | byParam

用法:
    # 标准向量自检
    python tools/decrypt.py --selftest

    # 解密一段 payload（hex 或文件），并按 msgid.csv 查表
    python tools/decrypt.py --payload "1a2b3c..." --key 133457799BBCDFF1
    python tools/decrypt.py --file cap.bin --key ascii:ZT2011KEY

    # 从 pcapng 里按 TCP 流导出 payload（需 tshark）
    python tools/decrypt.py --pcap captures/login.pcapng --port 7001 --key ...
"""

import argparse
import binascii
import csv
import os
import struct
import subprocess
import sys

# ---------------------------------------------------------------- DES core
IP = [58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
      62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
      57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
      61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7]
FP = [40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
      38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
      36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
      34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25]
E = [32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9, 8, 9, 10, 11, 12, 13,
     12, 13, 14, 15, 16, 17, 16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
     24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1]
P = [16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10,
     2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25]
PC1 = [57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18,
       10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36,
       63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22,
       14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4]
PC2 = [14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10,
       23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2,
       41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
       44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32]
SHIFT = [1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1]

SBOX = [
    [14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
     0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
     4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
     15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13],
    [15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
     3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
     0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
     13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9],
    [10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
     13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
     13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
     1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12],
    [7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
     13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
     10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
     3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14],
    [2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
     14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
     4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
     11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3],
    [12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
     10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
     9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
     4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13],
    [4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
     13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
     1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
     6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12],
    [13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
     1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
     7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
     2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11],
]


def _permute(table, block, in_bits):
    out = 0
    for pos in table:
        out = (out << 1) | ((block >> (in_bits - pos)) & 1)
    return out


def _subkeys(key8):
    k = int.from_bytes(key8, "big")
    cd = _permute(PC1, k, 64)
    c, d = cd >> 28, cd & 0x0FFFFFFF
    subs = []
    for s in SHIFT:
        c = ((c << s) | (c >> (28 - s))) & 0x0FFFFFFF
        d = ((d << s) | (d >> (28 - s))) & 0x0FFFFFFF
        subs.append(_permute(PC2, (c << 28) | d, 56))
    return subs


def _feistel(r, k):
    x = _permute(E, r, 32) ^ k
    out = 0
    for i in range(8):
        six = (x >> (42 - 6 * i)) & 0x3F
        row = ((six >> 4) & 0x2) | (six & 0x1)
        col = (six >> 1) & 0xF
        out = (out << 4) | SBOX[i][row * 16 + col]
    return _permute(P, out, 32)


def des_crypt(block8, subkeys, encrypt):
    b = int.from_bytes(block8, "big")
    b = _permute(IP, b, 64)
    l, r = b >> 32, b & 0xFFFFFFFF
    ks = subkeys if encrypt else list(reversed(subkeys))
    for k in ks:
        l, r = r, l ^ _feistel(r, k)
    b = _permute(FP, (r << 32) | l, 64)
    return b.to_bytes(8, "big")


def encdec(data: bytes, key: bytes, encrypt: bool) -> tuple:
    """复刻 CDes::encdec —— 每 8 字节一块，尾部余数不处理。返回 (结果, 余数字节数)。"""
    if len(key) != 8:
        raise ValueError("DES 密钥必须 8 字节")
    subs = _subkeys(key)
    buf = bytearray(data)
    off = 0
    n = len(buf)
    while off <= n - 8:
        buf[off:off + 8] = des_crypt(bytes(buf[off:off + 8]), subs, encrypt)
        off += 8
    return bytes(buf), n - off


# ---------------------------------------------------------------- RC5-32
# 来源：src/client-legacy/EncDec/include/myrc5.h + source/myrc5.cpp
# 参数：RC5-32/12/16（块 8 字节 / 12 轮 / 密钥 16 字节），小端装入
RC5_P = 0xB7E15163
RC5_Q = 0x9E3779B9
M32 = 0xFFFFFFFF

# 登录阶段密钥：src/client/RenderD3D/source/D3DFont.cpp:849（注释"客户端验证密钥"）
LOGIN_KEY = bytes.fromhex("1cc41924c27d56c4235cc2291ff126e0")


def _rotl(x, n):
    n &= 31
    return ((x << n) | (x >> (32 - n))) & M32


def _rotr(x, n):
    n &= 31
    return ((x >> n) | (x << (32 - n))) & M32


def rc5_setup(key: bytes, rounds: int = 12):
    c = max(1, (len(key) + 3) // 4)
    L = [0] * c
    for i in range(len(key)):
        L[i // 4] = (L[i // 4] | (key[i] << (8 * (i % 4)))) & M32
    t = 2 * (rounds + 1)
    S = [0] * t
    S[0] = RC5_P
    for i in range(1, t):
        S[i] = (S[i - 1] + RC5_Q) & M32
    A = B = i = j = 0
    for _ in range(3 * max(t, c)):
        A = S[i] = _rotl((S[i] + A + B) & M32, 3)
        B = L[j] = _rotl((L[j] + A + B) & M32, (A + B) & 31)
        i = (i + 1) % t
        j = (j + 1) % c
    return S


def rc5_block(block8, S, encrypt=True):
    a = int.from_bytes(block8[0:4], "little")
    b = int.from_bytes(block8[4:8], "little")
    n = len(S) // 2
    if encrypt:
        a = (a + S[0]) & M32
        b = (b + S[1]) & M32
        for i in range(1, n):
            a = (_rotl(a ^ b, b) + S[2 * i]) & M32
            b = (_rotl(b ^ a, a) + S[2 * i + 1]) & M32
    else:
        for i in range(n - 1, 0, -1):
            b = _rotr((b - S[2 * i + 1]) & M32, a) ^ a
            a = _rotr((a - S[2 * i]) & M32, b) ^ b
        b = (b - S[1]) & M32
        a = (a - S[0]) & M32
    return a.to_bytes(4, "little") + b.to_bytes(4, "little")


def rc5_encdec(data: bytes, key: bytes, encrypt: bool, rounds: int = 12):
    """与 CDes::encdec 同构：每 8 字节一块，尾部余数原样返回。"""
    S = rc5_setup(key, rounds)
    buf = bytearray(data)
    off, n = 0, len(buf)
    while off <= n - 8:
        buf[off:off + 8] = rc5_block(bytes(buf[off:off + 8]), S, encrypt)
        off += 8
    return bytes(buf), n - off


# ---------------------------------------------------------------- 帧层
# 来源：src/client/Client/Client.cpp:61-63
#   typedef unsigned long tPackLengthType;
#   #define PACKET_MAX_SIZE  0x00ffffff   // 数据包最大长度
#   #define PACKET_FLAG_ENC  0x80000000   // 数据包加密
#   #define PACKET_FLAG_ZIP  0x40000000   // 数据包压缩
# 发送侧 Client.cpp:427-431：
#   sendsize = ((datasize + 4 + 7) & ~7);          // 4 字节长度头一并参与加密，整体 8 对齐
#   *(DWORD*)senddata = (sendsize - 4) | flags;
#   *(DWORD*)senddata |= PACKET_FLAG_ENC;
# 接收侧 Client.cpp:165-232：先按 8 字节块解密，再读长度头，ZIP 则 uncompress。
FRAME_HDR = 4
PACKET_MAX_SIZE = 0x00FFFFFF
PACKET_FLAG_ENC = 0x80000000
PACKET_FLAG_ZIP = 0x40000000


def frame_build(body: bytes, zip_it: bool = True, key: bytes = None,
                algo: str = "des", rounds: int = 12, min_zip: int = 32) -> bytes:
    """复刻 CClient::Send 的封包：长度头 + 体，ZIP 阈值 32 字节，整体 8 对齐后加密。"""
    import zlib
    flags = 0
    payload = body
    if zip_it and len(body) > min_zip:
        payload = zlib.compress(body, 9)
        flags |= PACKET_FLAG_ZIP
    n = (len(payload) + FRAME_HDR + 7) & ~7
    buf = bytearray(n)
    struct.pack_into("<I", buf, 0, (n - FRAME_HDR) | flags)
    buf[FRAME_HDR:FRAME_HDR + len(payload)] = payload
    if key:
        flags |= PACKET_FLAG_ENC
        struct.pack_into("<I", buf, 0, struct.unpack_from("<I", buf, 0)[0] | PACKET_FLAG_ENC)
        if algo == "rc5":
            out, _ = rc5_encdec(bytes(buf), key, True, rounds)
        else:
            out, _ = encdec(bytes(buf), key, True)
        return out
    return bytes(buf)


def frame_parse(buf: bytes):
    """把已解密的字节流切成一帧一帧，返回 [(flags, length, body)] 与尾部残留长度。

    与 Client.cpp:165-232 一致：offset 前进 4 + length，不做 8 对齐回退。
    """
    out = []
    off = 0
    n = len(buf)
    while off + FRAME_HDR < n:
        raw = struct.unpack_from("<I", buf, off)[0]
        flags = raw & ~PACKET_MAX_SIZE
        length = raw & PACKET_MAX_SIZE
        if length == 0 or off + FRAME_HDR + length > n:
            break
        body = buf[off + FRAME_HDR: off + FRAME_HDR + length]
        if flags & PACKET_FLAG_ZIP:
            import zlib
            try:
                body = zlib.decompress(body)
            except Exception:
                pass
        out.append((flags, length, body))
        off += FRAME_HDR + length
    return out, n - off


# ---------------------------------------------------------------- 解包
def load_msgid(path="docs/protocol/msgid.csv"):
    table = {}
    if not os.path.exists(path):
        return table
    with open(path, encoding="utf-8") as f:
        for row in csv.DictReader(f):
            table[row["id"].upper().replace("0X", "0x")] = row
    return table


def parse_payload(data: bytes, table):
    out = []
    off = 0
    while off + 6 <= len(data):
        by_cmd, by_param = data[off], data[off + 1]
        ts = struct.unpack_from("<I", data, off + 2)[0]
        opcode = "0x%02X%02X" % (by_cmd, by_param)
        hit = table.get(opcode)
        out.append({
            "offset": off,
            "opcode": opcode,
            "name": hit["name"] if hit else "UNKNOWN",
            "len": int(hit["max_len"]) if hit else None,
            "timestamp": ts,
            "payload_hex": data[off + 6: off + 6 + 24].hex(),
        })
        step = int(hit["max_len"]) if hit and int(hit["max_len"]) > 0 else 8
        off += step
    return out


def from_pcap(path, port):
    """借 tshark 导出 TCP payload（未解密）。"""
    try:
        raw = subprocess.check_output([
            "tshark", "-r", path, "-Y", f"tcp.port=={port}",
            "-T", "fields", "-e", "tcp.payload"
        ]).decode().split()
    except Exception as e:
        print(f"[ERR] 调 tshark 失败: {e}", file=sys.stderr)
        return b""
    return b"".join(binascii.unhexlify(x) for x in raw if x)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--payload", help="十六进制字符串")
    ap.add_argument("--file", help="二进制文件")
    ap.add_argument("--pcap", help="pcapng 路径，用 tshark 取 payload")
    ap.add_argument("--port", type=int, default=7001)
    ap.add_argument("--key", default="", help="8 字节密钥；hex 或 ascii:XXXXXXXX")
    ap.add_argument("--algo", choices=["des", "rc5"], default="des",
                    help="des=登录后阶段（8 字节密钥）；rc5=登录阶段（16 字节密钥，默认硬编码）")
    ap.add_argument("--rounds", type=int, default=12, help="RC5 轮数，默认 12")
    ap.add_argument("--encrypt", action="store_true", help="反向加密（构造测试包）")
    ap.add_argument("--msgid", default="docs/protocol/msgid.csv")
    ap.add_argument("--no-frame", action="store_true",
                    help="跳过帧层解析（对端不带 4 字节长度头时用）")
    ap.add_argument("--selftest", action="store_true")
    a = ap.parse_args()

    if a.selftest:
        key = bytes.fromhex("133457799BBCDFF1")
        pt = bytes.fromhex("0123456789ABCDEF")
        ct = des_crypt(pt, _subkeys(key), True)
        ok1 = ct.hex().upper() == "85E813540F0AB405"
        back = des_crypt(ct, _subkeys(key), False)
        ok2 = back == pt
        print(f"DES 标准向量: {'通过' if ok1 else '失败'} ({ct.hex().upper()})")
        print(f"DES 往返一致: {'通过' if ok2 else '失败'}")

        # RC5-32/12/16 标准向量（RFC 2040）：key=全0, pt=全0 -> 21A5DBEE154B8F6D
        rk = rc5_setup(bytes(16), 12)
        rc = rc5_block(bytes(8), rk, True)
        ok3 = rc.hex().upper() == "21A5DBEE154B8F6D"
        rb = rc5_block(rc, rk, False)
        ok4 = rb == bytes(8)
        print(f"RC5 标准向量: {'通过' if ok3 else '失败'} ({rc.hex().upper()})")
        print(f"RC5 往返一致: {'通过' if ok4 else '失败'}")
        print(f"登录阶段密钥(硬编码): {LOGIN_KEY.hex()}")

        # 帧层往返：构造 3 条消息 → ZIP 压缩 → RC5 加密 → 解密 → 切帧 → 逐字节比对
        body = (b"\x01\x01" + struct.pack("<I", 0x11223344) + b"VER3.1.46.0\0")
        body += b"\x01\x02" + struct.pack("<I", 0x55667788) + bytes(34)
        body += b"\x0A\x05" + struct.pack("<I", 0x99AABBCC) + bytes(60)
        wire = frame_build(body, zip_it=True, key=LOGIN_KEY, algo="rc5", rounds=12)
        plain, _ = rc5_encdec(wire, LOGIN_KEY, False, 12)
        frames, tail = frame_parse(plain)
        ok5 = (len(frames) == 1 and frames[0][2] == body and tail == 0)
        ok6 = (len(wire) % 8 == 0)
        print(f"帧层往返一致: {'通过' if ok5 else '失败'} "
              f"(wire={len(wire)}B, 帧={len(frames)}, body={len(body)}B, tail={tail})")
        print(f"帧长 8 对齐: {'通过' if ok6 else '失败'} ({len(wire)} % 8 = {len(wire) % 8})")
        return 0 if all([ok1, ok2, ok3, ok4, ok5, ok6]) else 1

    if a.algo == "rc5" and not a.key:
        key = LOGIN_KEY
    elif a.key.startswith("ascii:"):
        key = a.key[6:].encode("latin-1")
    else:
        key = bytes.fromhex(a.key)
    need = 16 if a.algo == "rc5" else 8
    if len(key) != need:
        print(f"[ERR] {a.algo.upper()} 密钥必须 {need} 字节，当前 {len(key)}", file=sys.stderr)
        return 2

    if a.payload:
        data = binascii.unhexlify(a.payload.replace(" ", ""))
    elif a.file:
        data = open(a.file, "rb").read()
    elif a.pcap:
        data = from_pcap(a.pcap, a.port)
    else:
        print("[ERR] 需要 --payload / --file / --pcap 之一", file=sys.stderr)
        return 2

    if a.algo == "rc5":
        out, rest = rc5_encdec(data, key, a.encrypt, a.rounds)
    else:
        out, rest = encdec(data, key, a.encrypt)
    print(f"输入 {len(data)} 字节 → 处理 {len(data) - rest} 字节，尾部余数 {rest} 字节（按源码不加密）")
    print(f"明文前 64 字节: {out[:64].hex()}")

    table = load_msgid(a.msgid)

    if a.encrypt:
        # 加密方向只回显字节，不做帧解析
        print(f"输出 hex: {out.hex()}")
        return 0

    stream = out
    if not a.no_frame:
        frames, tail = frame_parse(out)
        if frames:
            print(f"帧层：切出 {len(frames)} 帧，尾部残留 {tail} 字节")
            for i, (flags, length, body) in enumerate(frames):
                tag = []
                if flags & PACKET_FLAG_ENC:
                    tag.append("ENC")
                if flags & PACKET_FLAG_ZIP:
                    tag.append("ZIP")
                print(f"  frame#{i}  len={length:<6} flags={','.join(tag) or '-'}  decompressed={len(body)}")
            stream = b"".join(b for _, _, b in frames)
    else:
        print("帧层：已按 --no-frame 跳过")

    print(f"msgid 表 {len(table)} 条")
    for m in parse_payload(stream, table):
        print(f"  +{m['offset']:<4} {m['opcode']}  {m['name']:<28} "
              f"len={m['len'] or '?'}  ts={m['timestamp']}  {m['payload_hex']}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
