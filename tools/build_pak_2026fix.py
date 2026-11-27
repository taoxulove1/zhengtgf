"""Build the furthest-known-good datas.pak for the 3.0.0.2606 client.

What it does:
  * takes the pristine 2022-02-15 pack (Data/datas.pak.pristine)
  * extracts datas/tables/characterstatebase.tbl and rewrites every record with
    4 trailing zero bytes, i.e. stride 572 -> 576.  The 2026 client's
    sizeof(CharacterStateBase_t) is 576 (proved by probe: 572 -> "人物状态表数据有误",
    576 -> the table check is passed).
  * repacks, leaving every other entry byte-identical.

Usage:
  python tools/build_pak_2026fix.py [out.pak]
"""
import sys, os, struct, zlib
sys.path.insert(0, r"E:\源端\zhengtugf\tools")
import pak_open, pak_repack, des_mydes as DES

PRISTINE = r"E:\源端\源1\Data\datas.pak.pristine"
OUT = r"E:\源端\tmp-paktest\datas-576.pak"
PASSWORD = b"xuzhao20041108"
TABLE = "datas/tables/characterstatebase.tbl"


def read_entry(pak_data, ent, ks):
    raw = pak_data[20 + ent["off"]:20 + ent["off"] + ent["size"]]
    head = bytearray(raw[:16])
    for i in range(0, 16, 8):
        head[i:i + 8] = DES.decrypt3(bytes(head[i:i + 8]), ks[0], ks[1], ks[2], DES.SP_DATA)
    ver, flags, usize, dcrc = struct.unpack("<IIII", bytes(head))
    payload = bytearray(raw[16:])
    if flags & pak_open.FLAG_ENCRYPT:
        for i in range(0, len(payload) - 7, 8):
            payload[i:i + 8] = DES.decrypt3(bytes(payload[i:i + 8]), ks[0], ks[1], ks[2],
                                            DES.SP_DATA)
    if flags & pak_open.FLAG_COMPRESS:
        return zlib.decompressobj(-15).decompress(bytes(payload))
    return bytes(payload)


def widen(raw, extra=4):
    n = struct.unpack_from("<I", raw, 0)[0]
    stride = (len(raw) - 4) // n
    out = bytearray(struct.pack("<I", n))
    for i in range(n):
        out += raw[4 + i * stride:4 + (i + 1) * stride] + b"\0" * extra
    return bytes(out), n, stride


def main(out=OUT):
    data = open(PRISTINE, "rb").read()
    info = pak_open.read_pak(PRISTINE, PASSWORD)
    ks = DES.make_key3(PASSWORD)
    ent = [e for e in info["entries"] if e["name"] == TABLE][0]
    raw = read_entry(data, ent, ks)
    wide, n, stride = widen(raw, 4)
    print("table: records=%d stride=%d -> %d  (%d -> %d bytes)"
          % (n, stride, stride + 4, len(raw), len(wide)))
    pak_repack.repack(PRISTINE, out, {TABLE: wide}, verbose=False)
    print("written", out, os.path.getsize(out))
    return out


if __name__ == "__main__":
    main(sys.argv[1] if len(sys.argv) > 1 else OUT)
