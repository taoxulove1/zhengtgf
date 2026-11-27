#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""frame_decode.py —— 把抓到的 pcap/pcapng 按帧层拆开并解出来。

用途：客户端侧（Wireshark/dumpcap）或服务端侧抓到的包，直接看出「谁发了什么 opcode、
多长、是否加密、是否压缩、压的是什么」。

帧格式（docs/protocol/protocol.md §0.1）：
    DWORD 长度头（小端）:
        bit31 PACKET_FLAG_ENC  = 加密
        bit30 PACKET_FLAG_ZIP  = zlib 压缩
        bit23-0 长度 = 帧总长 - 4（加密帧含零填充；明文帧不含，两种都要兼容）
    整帧 8 字节对齐后按 8 字节块加密（登录阶段 RC5-32/12/16，密钥硬编码）

用法：
    python tools/frame_decode.py captures/x.pcapng
    python tools/frame_decode.py captures/x.pcapng --port 8000 --hex 32
    python tools/frame_decode.py captures/x.pcapng --host 101.33.242.142
"""

import argparse
import importlib.util
import os
import struct
import sys
import zlib

HERE = os.path.dirname(os.path.abspath(__file__))


def load(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    m = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(m)
    return m


pcap = load("pcap_lite", os.path.join(HERE, "pcap_lite.py"))
dec = load("decrypt", os.path.join(HERE, "decrypt.py"))

FLAG_ENC = 0x80000000
FLAG_ZIP = 0x40000000
MAXLEN = 0x00FFFFFF


def align8(n):
    return (n + 7) & ~7


def parse_header(buf, off):
    """探测帧头。

    难点：ENC 标志打在长度头里，而长度头本身也参与加密 —— 所以加密帧必须先解 8 字节
    才知道长度。这里同时算「解出来的头」和「原始头」，用长度合法性 + ENC 位来决定信谁。

    返回 (hdr, enc) 或 (None, False)。
    """
    if off + 8 > len(buf):
        return None, False
    raw = struct.unpack_from("<I", buf, off)[0]
    try:
        pt, _ = dec.rc5_encdec(bytes(buf[off:off + 8]), dec.LOGIN_KEY, False)
        dhdr = struct.unpack_from("<I", pt, 0)[0]
    except Exception:
        dhdr = None

    def sane(h):
        if h is None:
            return False
        ln = h & MAXLEN
        return 1 <= ln <= (1 << 20)

    # 解出来的头带 ENC 位 -> 一定是加密帧
    if dhdr is not None and (dhdr & FLAG_ENC) and sane(dhdr):
        return dhdr, True
    # 原始头带 ENC 位（说明头是密文，位是巧合）-> 按加密处理
    if raw & FLAG_ENC and sane(dhdr):
        return dhdr, True
    # 原始头合理且解出来的不合理 -> 明文帧
    if sane(raw) and not sane(dhdr):
        return raw, False
    # 解出来的合理、原始不合理 -> 加密帧（没带 ENC 位的老服务端）
    if sane(dhdr) and not sane(raw):
        return dhdr, True
    if sane(raw):
        return raw, False
    return None, False


def split_frames(buf, key_hint=""):
    """把一条流的 payload 切成帧，返回 [(off, hdr, body, enc, zip, wire_len)]。"""
    frames = []
    off = 0
    n = len(buf)
    while off + 4 <= n:
        hdr, enc = parse_header(buf, off)
        if hdr is None:
            off += 1
            continue
        length = hdr & MAXLEN
        zip_ = bool(hdr & FLAG_ZIP)
        wire_len = align8(4 + length)
        if off + wire_len > n:
            frames.append((off, hdr, None, enc, zip_, wire_len))  # 半包
            break
        chunk = bytes(buf[off:off + align8(wire_len)])
        chunk = chunk[:wire_len]
        if enc:
            try:
                pt, _ = dec.rc5_encdec(chunk, dec.LOGIN_KEY, False)
                body = pt[4:4 + length]
            except Exception:
                body = chunk[4:4 + length]
        else:
            body = chunk[4:4 + length]
        frames.append((off, hdr, body, enc, zip_, wire_len))
        off += wire_len
    return frames


def describe(body, zip_):
    if body is None:
        return "半包(数据未到齐)"
    out = []
    compressed = body[2:4] == b"\x78\x9c"
    if compressed or zip_:
        payload = body[2:] if compressed else body
        try:
            raw = zlib.decompress(payload)
            if len(raw) >= 2:
                out.append(f"op={raw[0]:02x}{raw[1]:02x}")
            out.append(f"inflate={len(raw)}B head={raw[:10].hex()}")
        except Exception as e:
            if len(body) >= 2:
                out.append(f"op={body[0]:02x}{body[1]:02x}")
            out.append(f"inflate失败({e})")
    else:
        if len(body) >= 2:
            out.append(f"op={body[0]:02x}{body[1]:02x}")
        out.append(f"body={body[:10].hex()}")
    return "  ".join(out)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("path")
    ap.add_argument("--port", type=int, action="append", help="只看这些端口（可多次）")
    ap.add_argument("--host", default="", help="只看对端 IP")
    ap.add_argument("--hex", type=int, default=16, help="每个 body 头部打印字节数")
    a = ap.parse_args()

    ports = set(a.port) if a.port else None
    streams, order = pcap.build_streams(a.path, ports)
    if not order:
        print("没有带 payload 的 TCP 流")
        return 1

    shown = 0
    for key in order:
        src, sport, dst, dport = key
        if a.host and a.host not in (src, dst):
            continue
        buf = streams[key]
        flows = split_frames(buf)
        ok = [f for f in flows if f[2] is not None]
        print(f"\n=== {src}:{sport} -> {dst}:{dport}  payload={len(buf)}B  frames={len(ok)}")
        for i, (off, hdr, body, enc, zip_, wl) in enumerate(flows):
            tag = "ENC" if enc else "PLN"
            tag += "|ZIP" if zip_ else "    "
            ln = hdr & MAXLEN
            if body is None:
                print(f"  #{i:<3d} off={off:<6d} len={ln:<6d} {tag} 半包")
                continue
            head = body[:a.hex].hex()
            print(f"  #{i:<3d} off={off:<6d} len={ln:<6d} {tag} {describe(body, zip_)}")
            if a.hex:
                print(f"        head {head}")
        shown += 1
    if not shown:
        print("按过滤条件没有命中任何流")
    return 0


if __name__ == "__main__":
    sys.exit(main())
