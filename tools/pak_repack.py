"""Rebuild / patch an XPAK pack with our own writer (mirrors PackAggregate::MakePackDir).

Only the replaced entries are re-encrypted; every other entry's raw block is copied
byte for byte, so the result stays bit-identical for untouched files.
"""
import sys, os, zlib, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import des_mydes as DES
import pak_open

HDR = pak_open.HDR
ENT = pak_open.ENT
SIZEINFO = struct.Struct("<IIII")
PASSWORD = "xuzhao20041108"

_T = []
for _i in range(256):
    _v = _i
    for _ in range(8):
        _v = (0xEDB88320 ^ (_v >> 1)) if _v & 1 else _v >> 1
    _T.append(_v & 0xFFFFFFFF)


def crc(buf, seed=0xFFFFFFFF):
    c = seed
    for b in buf:
        c = _T[(c ^ b) & 0xFF] ^ (c >> 8)
    return c & 0xFFFFFFFF


def _enc(buf, ks, sp):
    buf = bytearray(buf)
    for i in range(0, len(buf) - 7, 8):
        buf[i:i + 8] = DES.encrypt3(bytes(buf[i:i + 8]), ks[0], ks[1], ks[2], sp)
    return bytes(buf)


def _dec(buf, ks, sp):
    buf = bytearray(buf)
    for i in range(0, len(buf) - 7, 8):
        buf[i:i + 8] = DES.decrypt3(bytes(buf[i:i + 8]), ks[0], ks[1], ks[2], sp)
    return bytes(buf)


def _build_block(orig, ks, sp, level, flag, no_compress=False):
    if no_compress:
        payload = orig
        per_flags = 0
    else:
        comp = zlib.compressobj(level, zlib.DEFLATED, -15)
        payload = comp.compress(orig) + comp.flush()
        per_flags = pak_open.FLAG_COMPRESS
    pad = (len(payload) + 7) // 8 * 8
    payload = payload + b"\0" * (pad - len(payload))
    if flag & pak_open.FLAG_ENCRYPT:
        per_flags |= pak_open.FLAG_ENCRYPT
    finfo = SIZEINFO.pack(9, per_flags, len(orig), crc(orig))
    if flag & pak_open.FLAG_ENCRYPT:
        finfo = _enc(finfo, ks, sp)
        payload = _enc(payload, ks, sp)
    block = finfo + payload
    inter = (len(block) + 3) & ~3
    block = block + b"\0" * (inter - len(block))
    return block, len(payload) + 16, inter, crc(payload, crc(finfo))


def repack(src, dst, replace=None, level=6, verbose=True, pad_last=0, add=None, raw=None):
    """replace: {normalized-name: new-bytes}; add: {normalized-name: bytes} appended."""
    replace = replace or {}
    raw = set(x.lower().replace("\\", "/") for x in (raw or ()))
    add = {k.lower().replace("\\", "/"): v for k, v in (add or {}).items()}
    info = pak_open.read_pak(src, PASSWORD.encode())
    data = open(src, "rb").read()
    ks = DES.make_key3(PASSWORD.encode())
    sp_d, sp_h = DES.SP_DATA, DES.SP_HDR
    flag = info["flag"]

    body = bytearray()
    metas = []
    for e in info["entries"]:
        off, size, inter = e["off"], e["size"], e["inter"]
        if e["name"] in replace:
            block, size_new, inter_new, crc_block = _build_block(
                replace[e["name"]], ks, sp_d, level, flag, e["name"] in raw)
            metas.append(dict(name=e["name"], id=crc(e["name"].encode()),
                              off=len(body), size=size_new, inter=inter_new,
                              crc=crc_block))
            body += block
            if verbose:
                print("  replaced %-45s %6d -> %6d bytes"
                      % (e["name"], len(replace[e["name"]]), size_new - 16))
        else:
            block = data[20 + off:20 + off + inter]
            metas.append(dict(name=e["name"], id=e["id"], off=len(body),
                              size=size, inter=inter, crc=e["crc"]))
            body += block

    for name, orig in add.items():
        block, size_new, inter_new, crc_block = _build_block(orig, ks, sp_d, level, flag)
        metas.append(dict(name=name, id=crc(name.encode()), off=len(body),
                          size=size_new, inter=inter_new, crc=crc_block))
        body += block
        if verbose:
            print("  added    %-45s %6d bytes" % (name, len(orig)))

    idx = bytearray()
    if pad_last:
        metas[-1]["inter"] += pad_last
        body += b"\0" * pad_last
    for m in metas:
        idx += ENT.pack(m["id"], m["off"], m["size"], m["inter"], m["crc"])
        idx += m["name"].encode("gbk") + b"\0"
    hsize = (len(idx) + 7) & ~7
    idx += b"\0" * (hsize - len(idx))
    if flag & (pak_open.FLAG_ENCRYPT | pak_open.FLAG_ENCRYPT_HDR):
        idx = bytearray(_enc(bytes(idx), ks, sp_h))

    header = HDR.pack(b"XPAK", 9, len(metas), hsize, flag)
    out = header + bytes(body) + bytes(idx)
    open(dst, "wb").write(out)
    if verbose:
        print("  wrote %s  %d bytes (body %d, index %d, flag %d)"
              % (dst, len(out), len(body), hsize, flag))
    return out


if __name__ == "__main__":
    import argparse
    ap = argparse.ArgumentParser()
    ap.add_argument("src")
    ap.add_argument("dst")
    ap.add_argument("--set", action="append", default=[],
                    help="name=localfile  (name uses / separators, lowercase)")
    a = ap.parse_args()
    rep = {}
    for spec in a.set:
        n, p = spec.split("=", 1)
        rep[n.lower().replace("\\", "/")] = open(p, "rb").read()
    repack(a.src, a.dst, rep)
