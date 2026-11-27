#!/usr/bin/env python3
"""Compare the native PING evidence with one normal PING on the original local FLServer."""

from pathlib import Path
import re
import socket
import subprocess
import sys
import time

from p3_runtime_smoke import FRAME_HEADER, PAYLOAD, REQUEST, REFERENCE_PREFIX, framed, read_exact


ORIGINAL_PORT = 8002


def native_evidence(log):
    lines = log.read_text(encoding="utf-8", errors="replace").splitlines()
    dirs = [line.split(" ", 1)[1] for line in lines if line.startswith("[RUNTIME_DIR] ")]
    if not dirs or not any(line.startswith("[OK][EXCHANGES] ") for line in lines):
        raise RuntimeError("Expected a completed native run in " + str(log))
    return Path(dirs[-1]).resolve(strict=True)


def original_listener():
    rows = subprocess.check_output(
        ["ss", "-lntp", "sport = :{}".format(ORIGINAL_PORT)], universal_newlines=True,
    )
    pids = set(re.findall(
        r'\("flserver_r\.exe",pid=(\d+),', rows, flags=re.IGNORECASE,
    ))
    if len(pids) != 1:
        raise RuntimeError("Expected one original FLServer_r.exe listener on TCP 8002")
    return next(iter(pids))


def compare(log):
    directory = native_evidence(log)
    hello = (directory / "ping-hello.bin").read_bytes()
    request = (directory / "ping-request.bin").read_bytes()
    native = (directory / "ping-reply.bin").read_bytes()
    expected_header = FRAME_HEADER.pack(len(REFERENCE_PREFIX) + 1)
    if (hello != framed(PAYLOAD.pack(30, 3, 0)) or request != framed(REQUEST)
            or native[:-1] != expected_header + REFERENCE_PREFIX):
        raise RuntimeError("Saved native PING files do not match the test case")

    pid = original_listener()
    print("[ORIGINAL][LISTENER] port={} pid={}".format(ORIGINAL_PORT, pid), flush=True)
    with socket.create_connection(("127.0.0.1", ORIGINAL_PORT), timeout=5) as connection:
        connection.sendall(hello)
        time.sleep(1)
        connection.sendall(request)
        header = read_exact(connection, FRAME_HEADER.size)
        size = FRAME_HEADER.unpack(header)[0] & 0x7FFFF
        if not 0 < size <= 4096:
            raise RuntimeError("Original PING header has an unexpected length: " + header.hex())
        original = header + read_exact(connection, size)

    (directory / "ping-original-reply.bin").write_bytes(original)
    if original_listener() != pid:
        raise RuntimeError("Original listener identity changed during the comparison")
    print("[HEX][REQUEST] {}".format(request.hex()))
    print("[HEX][NATIVE] {}".format(native.hex()))
    print("[HEX][ORIGINAL] {}".format(original.hex()))
    if original[:-1] != native[:-1]:
        raise RuntimeError("Original reply differs in fields other than the final state byte")
    print("[OK][ORIGINAL_STABLE_BYTES] matched={} skipped=state".format(len(native) - 1))
    if original != native:
        print("[P3_PENDING][ORIGINAL_RAW_BYTES] Only the final state byte differs")
        return 2
    print("[OK][ORIGINAL_RAW_BYTES] Entire reply matches ({} bytes)".format(len(native)))
    return 0


if __name__ == "__main__":
    try:
        if len(sys.argv) != 2:
            raise RuntimeError("Usage: python3 p3_runtime_reference.py p3-runtime-smoke.log")
        sys.exit(compare(Path(sys.argv[1])))
    except (OSError, RuntimeError, subprocess.SubprocessError) as error:
        print("[FAIL][ORIGINAL_BYTES] {}".format(error), file=sys.stderr)
        sys.exit(1)
