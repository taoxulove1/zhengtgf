#!/usr/bin/env python3
"""Compare saved native PING frames against a normal original-client capture."""

import argparse
from pathlib import Path
import struct
import sys

from p3_runtime_smoke import FRAME_HEADER
from pcap_lite import iter_packets, parse_tcp, strip_link


PORT = 8002
MAX_FRAME = 4096
PING_REQUEST = b"\x1e\x02"
PING_REPLY = b"\x1e\x01"


def port_streams(capture):
    streams = {}
    packet_count = 0
    sent_bytes = received_bytes = 0
    for _, link_type, packet in iter_packets(capture, strict=True):
        link = strip_link(link_type, packet)
        if link is None:
            continue
        tcp = parse_tcp(*link)
        if tcp is None or PORT not in (tcp["sport"], tcp["dport"]):
            continue
        packet_count += 1
        if tcp["sport"] == PORT:
            sent_bytes += len(tcp["payload"])
        else:
            received_bytes += len(tcp["payload"])
        if tcp["payload"]:
            key = tcp["src"], tcp["sport"], tcp["dst"], tcp["dport"]
            seq = (tcp["seq"] + bool(tcp["flags"] & 0x02)) & 0xFFFFFFFF
            streams.setdefault(key, []).append((seq, tcp["payload"]))
    print("[CAPTURE] tcp8002_packets={} client_payload_bytes={} server_payload_bytes={}".format(
        packet_count, received_bytes, sent_bytes))
    return streams, sent_bytes


def reassemble(parts):
    """Join the contiguous prefix, tolerating retransmissions and sequence wrap."""
    base = parts[0][0]
    ordered = sorted(((seq - base + (1 << 31)) % (1 << 32) - (1 << 31), data)
                     for seq, data in parts)
    start = ordered[0][0]
    result = bytearray()
    for offset, data in ordered:
        offset -= start
        if offset > len(result):
            break
        overlap = min(len(data), len(result) - offset)
        if result[offset:offset + overlap] != data[:overlap]:
            raise ValueError("Conflicting TCP payload at the same sequence")
        result.extend(data[overlap:])
    return bytes(result)


def frames(data):
    while len(data) >= FRAME_HEADER.size:
        size = FRAME_HEADER.unpack_from(data)[0]
        end = FRAME_HEADER.size + size
        if not 6 <= size <= MAX_FRAME or end > len(data):
            return
        yield data[:end]
        data = data[end:]


def zone(frame):
    return struct.unpack_from("<I", frame, FRAME_HEADER.size + 6)[0]


def compare(capture, native_request, native_reply):
    if (list(frames(native_request)) != [native_request] or len(native_request) != 14
            or native_request[4:6] != PING_REQUEST
            or list(frames(native_reply)) != [native_reply] or len(native_reply) != 31
            or native_reply[4:6] != PING_REPLY or zone(native_request) != zone(native_reply)):
        raise ValueError("Expected the native ping-request.bin and ping-reply.bin from one run")

    streams, sent_bytes = port_streams(capture)
    if not sent_bytes:
        print("[P3_PENDING][ORIGINAL_BYTES] No server payload on TCP 8002")
        return 2
    print("[CAPTURE] tcp8002_directions={}".format(len(streams)))
    observed_zones = set()
    matched_request = False
    for key, segments in streams.items():
        src, sport, dst, dport = key
        if dport != PORT or sport == PORT:
            continue
        requests = [frame for frame in frames(reassemble(segments))
                    if len(frame) == 14 and frame[4:6] == PING_REQUEST]
        observed_zones.update(zone(frame) for frame in requests)
        if native_request not in requests:
            continue
        matched_request = True
        if any(zone(frame) == zone(native_request) and frame != native_request for frame in requests):
            continue
        reverse = streams.get((dst, dport, src, sport), [])
        if not reverse:
            continue
        replies = (frame for frame in frames(reassemble(reverse))
                   if len(frame) == 31 and frame[4:6] == PING_REPLY
                   and zone(frame) == zone(native_request))
        original = next(replies, None)
        if original is None:
            continue
        print("[HEX][REQUEST] " + native_request.hex())
        print("[HEX][NATIVE] " + native_reply.hex())
        print("[HEX][ORIGINAL] " + original.hex())
        if original == native_reply:
            print("[OK][ORIGINAL_RAW_BYTES] {} bytes match".format(len(original)))
            return 0
        if original[:-1] == native_reply[:-1]:
            print("[P3_PENDING][ORIGINAL_RAW_BYTES] Only the final state byte differs")
        else:
            mismatch = next(i for i, pair in enumerate(zip(native_reply, original)) if pair[0] != pair[1])
            print("[P3_PENDING][ORIGINAL_RAW_BYTES] First differing byte offset={}".format(mismatch))
        return 2
    if matched_request:
        print("[P3_PENDING][ORIGINAL_BYTES] Matching request captured without a comparable reply")
    elif observed_zones:
        print("[P3_PENDING][ORIGINAL_BYTES] No byte-identical request. Captured zone IDs: {}. Native: 0x{:08x}".format(
            ", ".join("0x{:08x}".format(value) for value in sorted(observed_zones)), zone(native_request)))
    else:
        print("[P3_PENDING][ORIGINAL_BYTES] No recognized PING request on TCP 8002")
    return 2


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("capture", type=Path)
    parser.add_argument("native_request", type=Path, nargs="?")
    parser.add_argument("native_reply", type=Path, nargs="?")
    parser.add_argument("--inspect", action="store_true",
                        help="Check capture integrity and summarize TCP 8002 traffic")
    args = parser.parse_args()
    if args.inspect and (args.native_request or args.native_reply):
        parser.error("--inspect takes only the capture file")
    if not args.inspect and not (args.native_request and args.native_reply):
        parser.error("comparison requires both native PING files")
    try:
        if args.inspect:
            _, sent_bytes = port_streams(args.capture)
            if not sent_bytes:
                print("[P3_PENDING][ORIGINAL_BYTES] No server payload on TCP 8002")
                return 2
            return 0
        return compare(args.capture, args.native_request.read_bytes(), args.native_reply.read_bytes())
    except (OSError, ValueError, struct.error) as error:
        print("[FAIL][CAPTURE] {}".format(error), file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(main())
