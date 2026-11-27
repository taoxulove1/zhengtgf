"""Extract one entry from an XPAK pack (decrypt + optional inflate + CRC check).

Layout per entry (from PackAggregate.cpp):
  stFileSizeInfo { DWORD version; DWORD flags; DWORD uncompressedSize; DWORD dwCRC; }  (16 bytes,
  itself encrypted with the same 3DES when hdr.flag has encrypt bits)
  payload (filelen = entry.size - 16, encrypted with the same 3DES, padded to 8)
"""
import sys, os, zlib, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import des_mydes as DES
import pak_open

SIZEINFO = struct.Struct("<IIII")
PASSWORD = "xuzhao20041108"


def crc_table():
    t = []
    for i in range(256):
        c = i
        for _ in range(8):
            c = (c >> 1) ^ 0xEDB88320 if c & 1 else c >> 1
        t.append(c)
    return t


_CRC = crc_table()


def calc_crc(buf, seed=0):
    c = seed ^ 0xFFFFFFFF
    for b in buf:
        c = _CRC[(c ^ b) & 0xFF] ^ (c >> 8)
    return c ^ 0xFFFFFFFF


def extract(path, name, out_dir, sp_hdr=None, sp_data=None):
    info = pak_open.read_pak(path, PASSWORD.encode(), sp_hdr or DES.SP_HDR)
    entries = info["entries"]
    key = name.lower().replace("\\", "/")
    ent = None
    for e in entries:
        if e["name"].lower() == key:
            ent = e
            break
    if ent is None:
        raise KeyError("no entry %r in %s" % (name, path))
    data = open(path, "rb").read()
    base = 20 + ent["off"]
    ks = DES.make_key3(PASSWORD.encode())
    sp = sp_data or DES.SP_DATA
    raw = data[base:base + ent["size"]]
    head = bytearray(raw[:16])
    for i in range(0, 16, 8):
        head[i:i + 8] = DES.decrypt3(bytes(head[i:i + 8]), ks[0], ks[1], ks[2], sp)
    ver, flags, usize, dcrc = SIZEINFO.unpack(bytes(head))
    payload = bytearray(raw[16:])
    for i in range(0, len(payload) - 7, 8):
        payload[i:i + 8] = DES.decrypt3(bytes(payload[i:i + 8]), ks[0], ks[1], ks[2], sp)
    payload = bytes(payload)
    out = payload
    if flags & pak_open.FLAG_COMPRESS:
        d = zlib.decompressobj(-15)
        try:
            out = d.decompress(payload)
        except Exception:
            try:
                out = zlib.decompress(payload)
            except Exception as ex:
                out = b""
                print("  inflate failed:", ex)
    crc1 = calc_crc(raw[16:16 + (ent["size"] - 16)])
    crc2 = calc_crc(payload)
    print("entry=%s ver=%d flags=0x%x usize=%d dcrc=%08x enc_size=%d out_size=%d"
          % (ent["name"], ver, flags, usize, dcrc, ent["size"] - 16, len(out)))
    print("  crc(payload)=%08x crc(rawpayload)=%08x header_crc=%08x  match=%s"
          % (crc2, crc1, ent["crc"], "yes" if crc2 == ent["crc"] else "no"))
    os.makedirs(out_dir, exist_ok=True)
    op = os.path.join(out_dir, os.path.basename(ent["name"]))
    open(op, "wb").write(out)
    print("  written", op, len(out))
    return out


if __name__ == "__main__":
    out = extract(r"E:\源端\源1\Data\datas.pak",
                  "datas/tables/characterstatebase.tbl",
                  r"E:\源端\tmp-extract")
    print("first 64 bytes:", out[:64].hex())
    if len(out) >= 4:
        print("count field =", struct.unpack_from("<I", out, 0)[0])
