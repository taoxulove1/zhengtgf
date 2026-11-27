#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""o8002_probe.py —— 在服务器本机对原版 FlServer 8002 做分级探测并记录双向字节。

用法（服务器上）：
    python3 /root/o8002_probe.py             # 默认 127.0.0.1:8002
    python3 /root/o8002_probe.py --port 7002 # 打原生服务

分级：
    A 只连不发，看服务端是否主动推包
    B 发明文 0x0101 校验帧
    C 发 RC5 加密 0x0101 校验帧（客户端真实首包）
    D 发 8 字节空帧（长度头=4）
每级独立连接，输出完整 hex，便于逐字节比对。
"""
import argparse
import importlib.util
import os
import socket
import struct
import sys
import time

sys.path.insert(0, "/root")


def load(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    m = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(m)
    return m


dec = load("dec", "/root/decrypt.py")
hp = load("hp", "/root/handshake_probe.py")


def once(host, port, label, payload=None, wait=3.0):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.settimeout(wait)
    got = b""
    try:
        s.connect((host, port))
        if payload is not None:
            s.sendall(payload)
            print(f"[{label}] sent {len(payload)}B {payload.hex()}")
        t0 = time.time()
        while time.time() - t0 < wait:
            try:
                b = s.recv(8192)
            except socket.timeout:
                break
            if not b:
                print(f"[{label}] peer FIN after {len(got)}B")
                break
            got += b
        print(f"[{label}] recv {len(got)}B {got.hex()}")
        return got
    except Exception as e:
        print(f"[{label}] EXC {e!r}")
        return got
    finally:
        s.close()


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--host", default="127.0.0.1")
    ap.add_argument("--port", type=int, default=8002)
    ap.add_argument("--wait", type=float, default=3.0)
    a = ap.parse_args()

    print(f"=== target {a.host}:{a.port} ===")
    once(a.host, a.port, "A-bare", None, a.wait)

    plain = bytearray(16)
    body = bytes([0x01, 0x01]) + struct.pack("<I", int(time.time()) & 0xFFFFFFFF) + struct.pack("<I", 1999)
    struct.pack_into("<I", plain, 0, len(body))
    plain[4:4 + len(body)] = body
    once(a.host, a.port, "B-plain0101", bytes(plain), a.wait)

    rc5 = hp.build_verify_frame(dec, algo="rc5", version=1999)
    once(a.host, a.port, "C-rc50101", rc5, a.wait)

    empty = struct.pack("<I", 4) + b"\0\0\0\0"
    once(a.host, a.port, "D-empty", empty, a.wait)


if __name__ == "__main__":
    main()
