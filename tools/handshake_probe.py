#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""handshake_probe.py —— 对登录服做一次真实握手探测。

复刻客户端首次连接的动作（src/client/Client/Game.cpp:1023-1032）：
    连上 TCP 后立刻发 stUserVerifyVerCmd（opcode 0x0101，version 硬编码 1999）

帧格式（docs/protocol/protocol.md §0.1）：
    DWORD 长度头（little-endian，bit31=ENC，bit23-0 = 帧总长-4）+ body
    整帧 8 字节对齐后按 8 字节块加密（登录阶段 RC5-32/12/16）

用法：
    python tools/handshake_probe.py --host 101.33.242.142 --port 8000
    python tools/handshake_probe.py --host ... --algo none     # 明文发，验证服务端是否强制加密
    python tools/handshake_probe.py --host ... --raw "hex"     # 自定义载荷
"""

import argparse
import binascii
import importlib.util
import os
import socket
import struct
import sys
import time

HERE = os.path.dirname(os.path.abspath(__file__))


def _load_decrypt():
    spec = importlib.util.spec_from_file_location("dec", os.path.join(HERE, "decrypt.py"))
    m = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(m)
    return m


def build_verify_frame(dec, algo="rc5", version=1999, ts=None):
    """stNullUserCmd{byCmd=0x01,byParam=0x01,dwTimestamp} + DWORD version。"""
    ts = int(time.time()) & 0xFFFFFFFF if ts is None else ts
    body = bytes([0x01, 0x01]) + struct.pack("<I", ts) + struct.pack("<I", version)
    assert len(body) == 10
    if algo == "none":
        wire = bytearray(16)
        struct.pack_into("<I", wire, 0, len(body))
        wire[4:4 + len(body)] = body
        return bytes(wire)
    key = dec.LOGIN_KEY if algo == "rc5" else bytes(8)
    return dec.frame_build(body, zip_it=False, key=key, algo=algo)


def try_decode(dec, data):
    """返回 [(算法, 结果字节)]，逐个密钥试。"""
    out = []
    for name in ("rc5", "des_hard", "plain"):
        if name == "plain":
            out.append((name, data))
            continue
        try:
            if name == "rc5":
                buf, _ = dec.rc5_encdec(data, dec.LOGIN_KEY, False)
            else:
                buf, _ = dec.encdec(data, bytes(8), False)
            out.append((name, buf))
        except Exception as e:
            out.append((name, b"<err %s>" % str(e).encode()))
    return out


def dump(buf, limit=160):
    s = []
    for off in range(0, min(len(buf), limit), 16):
        chunk = buf[off:off + 16]
        s.append(f"  +{off:04x}  {chunk.hex(' '):<47}  "
                 + "".join(chr(c) if 32 <= c < 127 else "." for c in chunk))
    return "\n".join(s)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--host", required=True)
    ap.add_argument("--port", type=int, default=8000)
    ap.add_argument("--algo", choices=["rc5", "des", "none"], default="rc5")
    ap.add_argument("--version", type=int, default=1999)
    ap.add_argument("--wait", type=float, default=3.0, help="发完等回包秒数")
    ap.add_argument("--raw", default="", help="自定义 body（hex），给了就不发 0x0101")
    ap.add_argument("--timeout", type=float, default=6.0)
    a = ap.parse_args()

    dec = _load_decrypt()

    if a.raw:
        body = binascii.unhexlify(a.raw.replace(" ", ""))
        if a.algo == "none":
            wire = bytearray((len(body) + 4 + 7) & ~7)
            struct.pack_into("<I", wire, 0, len(body))
            wire[4:4 + len(body)] = body
            wire = bytes(wire)
        else:
            key = dec.LOGIN_KEY if a.algo == "rc5" else bytes(8)
            wire = dec.frame_build(body, zip_it=False, key=key, algo=a.algo)
    else:
        body = (bytes([0x01, 0x01]) + struct.pack("<I", int(time.time()) & 0xFFFFFFFF)
                + struct.pack("<I", a.version))
        wire = build_verify_frame(dec, a.algo, a.version)

    print(f"target   {a.host}:{a.port}")
    print(f"algo     {a.algo}")
    print(f"body     {len(body)}B  {body.hex(' ')}")
    print(f"wire     {len(wire)}B  {wire.hex(' ')}")

    s = socket.socket()
    s.settimeout(a.timeout)
    t0 = time.time()
    try:
        s.connect((a.host, a.port))
    except Exception as e:
        print(f"[FAIL] connect: {type(e).__name__}: {e}")
        return 2
    print(f"connect  ok  {(time.time()-t0)*1000:.1f} ms")

    # 先看服务端是否主动下发（有些登录服连上就推 0x0101/0x0104）
    s.settimeout(1.0)
    early = b""
    try:
        early = s.recv(4096)
    except socket.timeout:
        pass
    except Exception:
        pass
    if early:
        print(f"\n[服务端主动下发] {len(early)}B")
        print(dump(early))
        for name, buf in try_decode(dec, early):
            print(f"  decode as {name}: {buf[:48].hex(' ')}")
    else:
        print("服务端未主动下发（正常，等客户端先发）")

    s.sendall(wire)
    print(f"\nsent {len(wire)}B, 等待 {a.wait}s 回包")
    s.settimeout(a.wait)
    resp = b""
    try:
        while True:
            chunk = s.recv(4096)
            if not chunk:
                print("对端关闭连接 (FIN)")
                break
            resp += chunk
    except socket.timeout:
        pass
    except Exception as e:
        print(f"recv: {type(e).__name__}: {e}")

    if not resp:
        print("[无回包] 服务端未响应 0x0101 —— 可能密钥不对、端口不是登录口、或直接丢弃")
        s.close()
        return 3

    print(f"\n[回包] {len(resp)}B")
    print(dump(resp))
    print("\n逐算法试解：")
    for name, buf in try_decode(dec, resp):
        print(f"--- {name}")
        print(dump(buf, 80))
        if len(buf) >= 2:
            print(f"    opcode 猜测 0x{buf[0]:02X}{buf[1]:02X}")
    s.close()
    return 0


if __name__ == "__main__":
    sys.exit(main())
