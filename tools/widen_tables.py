"""Widen every .tbl in datas.pak by N extra bytes per record (probe for short reads)."""
import sys, os, struct, zlib
sys.path.insert(0, r"E:\源端\zhengtugf\tools")
import pak_open, pak_repack, des_mydes as DES

PRISTINE = r"E:\源端\源1\Data\datas.pak.pristine"
PASSWORD = b"xuzhao20041108"


def table_stride(data, ent, ks):
    raw = data[20 + ent["off"]:20 + ent["off"] + ent["size"]]
    head = bytearray(raw[:16])
    for i in range(0, 16, 8):
        head[i:i + 8] = DES.decrypt3(bytes(head[i:i + 8]), ks[0], ks[1], ks[2], DES.SP_DATA)
    ver, flags, usize, dcrc = struct.unpack("<IIII", bytes(head))
    payload = bytearray(raw[16:])
    if flags & 2:
        for i in range(0, len(payload) - 7, 8):
            payload[i:i + 8] = DES.decrypt3(bytes(payload[i:i + 8]), ks[0], ks[1], ks[2],
                                            DES.SP_DATA)
    out = zlib.decompressobj(-15).decompress(bytes(payload)) if flags & 1 else bytes(payload)
    return out


def widen_all(extra=4, out=None):
    data = open(PRISTINE, "rb").read()
    info = pak_open.read_pak(PRISTINE, PASSWORD)
    ks = DES.make_key3(PASSWORD)
    rep = {}
    for e in info["entries"]:
        if not e["name"].endswith(".tbl"):
            continue
        raw = table_stride(data, e, ks)
        n = struct.unpack_from("<I", raw, 0)[0]
        stride = (len(raw) - 4) // n
        buf = bytearray(struct.pack("<I", n))
        for i in range(n):
            buf += raw[4 + i * stride:4 + (i + 1) * stride] + b"\0" * extra
        rep[e["name"]] = bytes(buf)
    pak_repack.repack(PRISTINE, out, replace=rep, verbose=False)
    print("built", out, os.path.getsize(out), "tables:", len(rep))
    return out


if __name__ == "__main__":
    widen_all(4, r"E:\源端\tmp-paktest\all-plus4.pak")
