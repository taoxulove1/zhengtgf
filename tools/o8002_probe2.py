#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""o8002_probe2.py —— 对 PING 口做载荷矩阵探测。

背景：config.xml 里 `<ping_port>8002</ping_port>` 注释为「客户端PING端口，外部使用」，
      `<inside_port>8001</inside_port>` 为内部端口。
      8002 在 `127.0.0.1` 上连上即 FIN；服务器本机 eth0 是 10.1.0.10，
      故改用 10.1.0.10 再跑一轮，并铺开载荷矩阵，看哪种载荷能拿到回包。

用法：
    python3 /root/o8002_probe2.py --host 10.1.0.10 --port 8002
"""
import argparse
import importlib.util
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


def frame(body, algo="none"):
    """按 §0.1 组帧：4B 长度头 + body，8 字节对齐。"""
    if algo == "none":
        n = (len(body) + 4 + 7) & ~7
        buf = bytearray(n)
        struct.pack_into("<I", buf, 0, n - 4)
        buf[4:4 + len(body)] = body
        return bytes(buf)
    return dec.frame_build(body, zip_it=False, key=dec.LOGIN_KEY, algo="rc5")


def payloads():
    ts = int(time.time()) & 0xFFFFFFFF
    out = []
    out.append(("bare", None))
    out.append(("ts4", struct.pack("<I", ts)))
    out.append(("ts4x2", struct.pack("<II", ts, ts)))
    out.append(("ping-literal", b"PING" + struct.pack("<I", ts)))
    for op, param in ((0x01, 0x00), (0x01, 0x01), (0x01, 0x02), (0x02, 0x01), (0x00, 0x00)):
        body = bytes([op, param]) + struct.pack("<I", ts)
        out.append((f"plain-{op:02x}{param:02x}-6B", frame(body)))
        out.append((f"rc5-{op:02x}{param:02x}-6B", frame(body, "rc5")))
    out.append(("verify-plain", frame(bytes([0x01, 0x01]) + struct.pack("<I", ts) + struct.pack("<I", 1999))))
    out.append(("verify-rc5", hp.build_verify_frame(dec, algo="rc5", version=1999)))
    return out


def once(host, port, label, payload, wait):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.settimeout(wait)
    got = b""
    try:
        s.connect((host, port))
        if payload is not None:
            s.sendall(payload)
        t0 = time.time()
        while time.time() - t0 < wait:
            try:
                b = s.recv(8192)
            except socket.timeout:
                break
            if not b:
                break
            got += b
        verdict = f"recv {len(got)}B" if got else "no-data"
        print(f"[{label:22s}] sent={0 if payload is None else len(payload):>3d}B  {verdict}  {got.hex()}")
        return got
    except Exception as e:
        print(f"[{label:22s}] EXC {e!r}")
        return got
    finally:
        s.close()


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--host", default="10.1.0.10")
    ap.add_argument("--port", type=int, default=8002)
    ap.add_argument("--wait", type=float, default=2.5)
    a = ap.parse_args()
    print(f"=== {a.host}:{a.port} 载荷矩阵 ===")
    hits = 0
    for label, p in payloads():
        if once(a.host, a.port, label, p, a.wait):
            hits += 1
    print(f"=== 有回包 {hits} 组 ===")


if __name__ == "__main__":
    main()
