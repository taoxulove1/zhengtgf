#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""pcap_lite.py —— 纯 Python 的 pcap / pcapng 读取 + TCP 流重组。

不依赖 tshark / scapy / dpkt。目标是让 P1 抓包在任何一台机器上都能出结论：
    读 pcapng/pcap → 解 Ethernet / Linux SLL / SLL2 / raw IP → IPv4 / IPv6
    → TCP → 按四元组重组 payload → 交给 tools/decrypt.py 解帧

支持：
    SHB 0x0A0D0D0A  段头
    IDB 0x00000001  接口描述（linktype / snaplen）
    EPB 0x00000006  增强包（含时间戳）
    SPB 0x00000003  简单包（无时间戳）
    pcap 经典格式（magic 0xa1b2c3d4 / 0xd4c3b2a1 微秒、0xa1b23c4d 纳秒）

用法：
    python tools/pcap_lite.py captures/x.pcapng --list
    python tools/pcap_lite.py captures/x.pcapng --stream 0 --out stream0.bin
    python tools/pcap_lite.py captures/x.pcapng --flow "8000" --hexdump 128
    python tools/pcap_lite.py captures/x.pcapng --selftest
"""

import argparse
import io
import os
import struct
import sys

# ------------------------------------------------------------------ 块解析
LINKTYPE_NULL = 0
LINKTYPE_ETHERNET = 1
LINKTYPE_RAW = 101
LINKTYPE_LINUX_SLL = 113
LINKTYPE_LINUX_SLL2 = 276


def read_pcapng(fh, strict=False):
    """产出 (ts_epoch_float, linktype, raw_bytes)。"""
    data = fh.read()
    off = 0
    n = len(data)
    linktypes = []
    while off < n:
        if off + 12 > n:
            if strict:
                raise ValueError(f"Incomplete pcapng block header at byte {off}")
            break
        btype, blen = struct.unpack_from("<II", data, off)
        if blen < 12 or off + blen > n:
            if strict:
                raise ValueError(f"Incomplete or invalid pcapng block at byte {off}")
            break
        if strict and (blen % 4 or struct.unpack_from("<I", data, off + blen - 4)[0] != blen):
            raise ValueError(f"Invalid pcapng block length at byte {off}")
        body = data[off + 8: off + blen - 4]
        if btype == 0x0A0D0D0A:            # SHB
            if strict and (len(body) < 16 or body[:4] != b"\x4d\x3c\x2b\x1a"):
                raise ValueError(f"Unsupported or invalid pcapng section at byte {off}")
            linktypes = []
        elif btype == 0x00000001:          # IDB
            if strict and len(body) < 8:
                raise ValueError(f"Incomplete pcapng interface at byte {off}")
            lt = struct.unpack_from("<H", body, 0)[0]
            linktypes.append(lt)
        elif btype == 0x00000006:          # EPB
            iface, tsh, tsl, caplen, _orig = struct.unpack_from("<IIIII", body, 0)
            if strict and (iface >= len(linktypes) or caplen > len(body) - 20 or caplen != _orig):
                raise ValueError(f"Invalid interface or incomplete pcapng packet at byte {off}")
            ts = ((tsh << 32) | tsl) / 1e6
            pkt = body[20:20 + caplen]
            lt = linktypes[iface] if iface < len(linktypes) else LINKTYPE_ETHERNET
            yield ts, lt, pkt
        elif btype == 0x00000003:          # SPB
            caplen = struct.unpack_from("<I", body, 0)[0]
            if strict and (not linktypes or caplen > len(body) - 4):
                raise ValueError(f"Invalid interface or incomplete pcapng packet at byte {off}")
            pkt = body[4:4 + caplen]
            lt = linktypes[0] if linktypes else LINKTYPE_ETHERNET
            yield 0.0, lt, pkt
        off += blen


def read_pcap(fh, strict=False):
    hdr = fh.read(24)
    if len(hdr) < 24:
        if strict:
            raise ValueError("Incomplete pcap file header")
        return
    magic = struct.unpack_from("<I", hdr, 0)[0]
    if magic == 0xA1B2C3D4:
        endian, nano = "<", False
    elif magic == 0xD4C3B2A1:
        endian, nano = ">", False
    elif magic == 0xA1B23C4D:
        endian, nano = "<", True
    elif magic == 0x4D3CB2A1:
        endian, nano = ">", True
    else:
        raise ValueError(f"未知 pcap magic 0x{magic:08X}")
    snaplen, linktype = struct.unpack_from(endian + "II", hdr, 16)
    # The CentOS captures store the synthetic Linux cooked header in the
    # captured length while leaving it out of the original length.
    cooked_header_size = {LINKTYPE_LINUX_SLL: 16, LINKTYPE_LINUX_SLL2: 20}.get(linktype, 0)
    while True:
        offset = fh.tell()
        rec = fh.read(16)
        if len(rec) < 16:
            if strict and rec:
                raise ValueError(f"Incomplete pcap packet header at byte {offset} ({len(rec)} of 16 bytes)")
            break
        ts_s, ts_frac, caplen, original_size = struct.unpack(endian + "IIII", rec)
        if strict:
            if caplen < original_size:
                raise ValueError(f"Truncated pcap packet at byte {offset} ({caplen} of {original_size} bytes)")
            if caplen > snaplen or caplen - original_size not in (0, cooked_header_size):
                raise ValueError(f"Invalid pcap packet length at byte {offset} ({caplen} bytes)")
        pkt = fh.read(caplen)
        if len(pkt) < caplen:
            if strict:
                raise ValueError(f"Incomplete pcap packet data at byte {offset} ({len(pkt)} of {caplen} bytes)")
            break
        div = 1e9 if nano else 1e6
        yield ts_s + ts_frac / div, linktype, pkt


def iter_packets(path, strict=False):
    """Read a capture, optionally rejecting incomplete records or packet payloads."""
    with open(path, "rb") as fh:
        magic = fh.read(4)
        fh.seek(0)
        if magic == b"\x0a\x0d\x0d\x0a":
            yield from read_pcapng(fh, strict=strict)
        else:
            yield from read_pcap(fh, strict=strict)


# ------------------------------------------------------------------ 解码
def strip_link(linktype, pkt):
    """返回 (ethertype_or_None, l3bytes) 或 None。"""
    if linktype == LINKTYPE_ETHERNET:
        if len(pkt) < 14:
            return None
        etype = struct.unpack_from(">H", pkt, 12)[0]
        off = 14
        # 802.1Q
        while etype == 0x8100 and len(pkt) >= off + 4:
            etype = struct.unpack_from(">H", pkt, off + 2)[0]
            off += 4
        return etype, pkt[off:]
    if linktype == LINKTYPE_LINUX_SLL:
        if len(pkt) < 16:
            return None
        return struct.unpack_from(">H", pkt, 14)[0], pkt[16:]
    if linktype == LINKTYPE_LINUX_SLL2:
        if len(pkt) < 20:
            return None
        return struct.unpack_from(">H", pkt, 0)[0], pkt[20:]
    if linktype in (LINKTYPE_RAW, LINKTYPE_NULL):
        if not pkt:
            return None
        ver = pkt[0] >> 4
        return (0x0800 if ver == 4 else 0x86DD), pkt
    return None


def parse_tcp(etype, l3):
    """返回 dict 或 None。"""
    if etype == 0x0800:
        if len(l3) < 20:
            return None
        ihl = (l3[0] & 0x0F) * 4
        if l3[9] != 6:
            return None
        total = struct.unpack_from(">H", l3, 2)[0]
        src = ".".join(str(b) for b in l3[12:16])
        dst = ".".join(str(b) for b in l3[16:20])
        tcp = l3[ihl:total] if total >= ihl else l3[ihl:]
    elif etype == 0x86DD:
        if len(l3) < 40 or l3[6] != 6:
            return None
        plen = struct.unpack_from(">H", l3, 4)[0]
        src = ":".join("%02x%02x" % (l3[8 + i], l3[9 + i]) for i in range(0, 16, 2))
        dst = ":".join("%02x%02x" % (l3[24 + i], l3[25 + i]) for i in range(0, 16, 2))
        tcp = l3[40:40 + plen]
    else:
        return None
    if len(tcp) < 20:
        return None
    sport, dport = struct.unpack_from(">HH", tcp, 0)
    seq = struct.unpack_from(">I", tcp, 4)[0]
    doff = (tcp[12] >> 4) * 4
    flags = tcp[13]
    payload = tcp[doff:] if len(tcp) >= doff else b""
    return {"src": src, "dst": dst, "sport": sport, "dport": dport,
            "seq": seq, "flags": flags, "payload": payload}


FLAG_NAMES = [(0x02, "S"), (0x10, "A"), (0x01, "F"), (0x08, "P"), (0x04, "R"), (0x20, "U")]


def flags_str(f):
    s = "".join(n for b, n in FLAG_NAMES if f & b)
    return s or "-"


# ------------------------------------------------------------------ 流重组
def build_streams(path, ports=None):
    """按 (src,sport,dst,dport) 归并 payload，返回 {key: bytearray}。"""
    streams = {}
    order = []
    for _ts, lt, pkt in iter_packets(path):
        st = strip_link(lt, pkt)
        if not st:
            continue
        t = parse_tcp(*st)
        if not t or not t["payload"]:
            continue
        if ports and t["sport"] not in ports and t["dport"] not in ports:
            continue
        key = (t["src"], t["sport"], t["dst"], t["dport"])
        if key not in streams:
            streams[key] = bytearray()
            order.append(key)
        streams[key].extend(t["payload"])
    return streams, order


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("path", nargs="?")
    ap.add_argument("--list", action="store_true", help="列出所有 TCP 流")
    ap.add_argument("--stream", type=int, default=-1, help="导出第 N 条流的 payload")
    ap.add_argument("--flow", default="", help="只保留端口含该值（逗号分隔）的流")
    ap.add_argument("--out", default="")
    ap.add_argument("--hexdump", type=int, default=0, help="打印前 N 字节 hex")
    ap.add_argument("--selftest", action="store_true")
    a = ap.parse_args()

    if a.selftest:
        return selftest()

    if not a.path:
        print("[ERR] 需要 pcapng/pcap 路径", file=sys.stderr)
        return 2
    ports = set(int(x) for x in a.flow.split(",") if x.strip()) if a.flow else None
    streams, order = build_streams(a.path, ports)
    print(f"{os.path.basename(a.path)}: {len(order)} 条 TCP 流"
          + (f"（端口过滤 {sorted(ports)}）" if ports else ""))
    if a.list or a.stream < 0:
        for i, k in enumerate(order):
            print(f"  #{i:<3} {k[0]}:{k[1]} -> {k[2]}:{k[3]}  {len(streams[k])} B  "
                  f"{streams[k][:16].hex()}")
    if a.stream >= 0:
        if a.stream >= len(order):
            print("[ERR] 流号越界", file=sys.stderr)
            return 2
        buf = streams[order[a.stream]]
        print(f"流 #{a.stream} payload {len(buf)} 字节")
        if a.out:
            with open(a.out, "wb") as f:
                f.write(buf)
            print(f"已写 {a.out}")
        if a.hexdump:
            for off in range(0, min(len(buf), a.hexdump), 16):
                chunk = buf[off:off + 16]
                print(f"  +{off:04x}  {chunk.hex(' '):<47}  "
                      + "".join(chr(c) if 32 <= c < 127 else "." for c in chunk))
    return 0


def selftest():
    """自造 pcapng（Linux SLL + IPv4 + TCP）验证解析链。"""
    def pkt(payload, sport=12345, dport=8000, seq=1):
        tcp = struct.pack(">HHIIBBHHH", sport, dport, seq, 0, 0x50, 0x18, 8192, 0, 0)
        total = 20 + len(tcp) + len(payload)
        ip = struct.pack(">BBHHHBBH4s4s", 0x45, 0, total, 0, 0,
                         64, 6, 0, bytes([127, 0, 0, 1]), bytes([127, 0, 0, 1]))
        sll = struct.pack(">HHH", 0, 1, 6) + b"\x00" * 8 + struct.pack(">H", 0x0800)
        return sll + ip + tcp + payload

    body = b"HELLO-STREAM"
    p1 = pkt(body[:6], seq=1)
    p2 = pkt(body[6:], seq=7)
    blocks = b""
    shb_body = struct.pack("<IHHq", 0x1A2B3C4D, 1, 0, -1)
    blocks += struct.pack("<II", 0x0A0D0D0A, 12 + len(shb_body)) + shb_body + struct.pack("<I", 12 + len(shb_body))
    idb_body = struct.pack("<HHI", LINKTYPE_LINUX_SLL, 0, 65535)
    blocks += struct.pack("<II", 1, 12 + len(idb_body)) + idb_body + struct.pack("<I", 12 + len(idb_body))
    for i, p in enumerate([p1, p2]):
        pad = (4 - len(p) % 4) % 4
        body_epb = struct.pack("<IIIII", 0, 0, 1600000000 + i, len(p), len(p)) + p + b"\x00" * pad
        blocks += struct.pack("<II", 6, 12 + len(body_epb)) + body_epb + struct.pack("<I", 12 + len(body_epb))

    tmp = os.path.join(os.environ.get("TEMP", "/tmp"), "pcap_lite_selftest.pcapng")
    with open(tmp, "wb") as f:
        f.write(blocks)
    streams, order = build_streams(tmp)
    ok = len(order) == 1 and bytes(streams[order[0]]) == body
    print(f"pcapng 解析: {'通过' if ok else '失败'} (流={len(order)}, "
          f"payload={bytes(streams[order[0]]) if order else b''})")
    print(f"四元组: {order[0] if order else '-'}")
    os.unlink(tmp)
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
