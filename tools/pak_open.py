"""Open (decrypt + parse) an XPAK pack using the ported mydes.cpp.

Usage:
  python tools/pak_open.py <pak> [--list] [--find substring] [--passwords a,b]
"""
import sys, os, struct, json
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import des_mydes as DES

HDR = struct.Struct("<4siiii")          # magic, version, filenum, headerSize, flag
ENT = struct.Struct("<IIIII")           # dwFileID, fileOffset, size, interspaceSize, dwCRC
FLAG_COMPRESS, FLAG_ENCRYPT, FLAG_ENCRYPT_HDR = 1, 2, 4


def read_pak(path, password, sp=None):
    data = open(path, "rb").read()
    magic, ver, filenum, hsize, flag = HDR.unpack_from(data, 0)
    if magic != b"XPAK":
        raise ValueError("not an XPAK: %r" % magic)
    tail = bytearray(data[len(data) - hsize:])
    if flag & (FLAG_ENCRYPT | FLAG_ENCRYPT_HDR):
        sp = sp or DES.SP_HDR
        ks = DES.make_key3(password)
        for i in range(0, len(tail) - 7, 8):
            tail[i:i + 8] = DES.decrypt3(bytes(tail[i:i + 8]), ks[0], ks[1], ks[2], sp)
    entries = []
    p = 0
    for _ in range(filenum):
        fid, off, size, inter, crc = ENT.unpack_from(tail, p)
        p += ENT.size
        e = tail.index(b"\0", p)
        name = tail[p:e].decode("gbk", "replace")
        p = e + 1
        entries.append(dict(id=fid, off=off, size=size, inter=inter, crc=crc, name=name))
    return dict(path=path, version=ver, filenum=filenum, headerSize=hsize,
                flag=flag, size=len(data), entries=entries, consumed=p)


def score(entries):
    if not entries:
        return 0.0
    good = 0
    for e in entries:
        n = e["name"]
        if not n:
            continue
        ok = sum(1 for ch in n if ch.isascii() and (ch.isalnum() or ch in "\\/._-"))
        if ok == len(n) and len(n) >= 3:
            good += 1
    return good / len(entries)


PASSWORDS = ["20210112taozhi", "xuzhao20041108"]


def open_best(path, passwords=None):
    best = None
    for pw in (passwords or PASSWORDS):
        try:
            info = read_pak(path, pw.encode())
        except Exception as ex:
            print("  pw %-16s -> error %s" % (pw, ex))
            continue
        s = score(info["entries"])
        print("  pw %-16s -> printable-name ratio %.3f  first=%r"
              % (pw, s, info["entries"][0]["name"] if info["entries"] else None))
        if best is None or s > best[0]:
            best = (s, pw, info)
    return best


if __name__ == "__main__":
    args = sys.argv[1:]
    if not args:
        print(__doc__)
        sys.exit(1)
    pak = args[0]
    do_list = "--list" in args
    find = None
    if "--find" in args:
        find = args[args.index("--find") + 1]
    print("pak:", pak, os.path.getsize(pak))
    best = open_best(pak)
    if not best:
        sys.exit(2)
    s, pw, info = best
    print("best password=%s ratio=%.3f version=%d files=%d hdr=%d flag=0x%x consumed=%d/%d"
          % (pw, s, info["version"], info["filenum"], info["headerSize"],
             info["flag"], info["consumed"], info["headerSize"]))
    if find:
        hits = [e for e in info["entries"] if find.lower() in e["name"].lower()]
        print("matches for %r: %d" % (find, len(hits)))
        for e in hits[:20]:
            print("   id=%d off=%d size=%d crc=%08x %s"
                  % (e["id"], e["off"], e["size"], e["crc"], e["name"]))
    if do_list:
        for e in info["entries"]:
            print("   id=%d off=%d size=%d %s" % (e["id"], e["off"], e["size"], e["name"]))
