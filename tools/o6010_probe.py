#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""o6010_probe.py —— 对 6010 场景服做 PING 三步握手验证。

协议（tools/frame_decode.py 与 P3 抓包结论）：
  hello   10B: 06000000 1e03 00000000        （客户端 -> 服务）
  request 14B: 0a000000 1e02 00000000 01000100（服务 -> 客户端）
  reply   31B: 1b000000 1e01 ...             （客户端 -> 服务）
PingTask::verifyConn() 要求首包是 0x1E + param3，否则连接被回收。
"""
import socket
import struct
import sys

HOST = "101.33.242.142"
PORT = 6010

def build_hello():
    # opcode 0x1E, param0=3, param1=0, param2=0 -> 4(len)+2+4+4 = 14? 实测抓包 10B:
    # 06 00 00 00 | 1e 03 | 00 00 00 00
    body = struct.pack("<BB", 0x1E, 0x03) + struct.pack("<I", 0)
    hdr = struct.pack("<I", len(body))
    return hdr + body

def main():
    s = socket.create_connection((HOST, PORT), timeout=8)
    s.settimeout(8)
    hello = build_hello()
    s.sendall(hello)
    print("-> sent %dB: %s" % (len(hello), hello.hex()))
    try:
        data = s.recv(256)
    except socket.timeout:
        print("<- no reply (timeout)")
        return 1
    print("<- recv %dB: %s" % (len(data), data.hex()))
    if len(data) >= 14 and data[4] == 0x1E and data[5] == 0x02:
        print("[RESULT] 6010 PING 握手正常：服务端回了 request(0x1E/0x02)")
        # 回 reply：1b000000 1e01 00000000 01000100 + 16B 零 + 00
        reply = struct.pack("<I", 27) + struct.pack("<BB", 0x1E, 0x01) + struct.pack("<I", 0) \
            + struct.pack("<BB", 0x01, 0x00) + struct.pack("<BB", 0x01, 0x00) + b"\x00" * 16 + b"\x00"
        s.sendall(reply)
        print("-> sent reply %dB" % len(reply))
        return 0
    print("[RESULT] 收到非预期回包，opcode=%s" % (data[4:6].hex() if len(data) >= 6 else "-"))
    return 1

if __name__ == "__main__":
    sys.exit(main())
