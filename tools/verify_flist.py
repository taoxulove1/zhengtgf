"""Compare local files against flist.xml (size + engine CRC) to find mismatches."""
import os, re, sys
sys.path.insert(0, r"E:\源端\zhengtugf\tools")
from pak_repack import crc

ROOT = r"E:\源端\源1"
FLIST = os.path.join(ROOT, "flist.xml")


def file_crc(p):
    c = 0xFFFFFFFF
    with open(p, "rb") as f:
        while True:
            b = f.read(1 << 20)
            if not b:
                break
            c = crc(b, c)
    return c & 0xFFFFFFFF


def main():
    txt = open(FLIST, "r", encoding="gb2312", errors="surrogateescape").read()
    ents = re.findall(r'<file size="(\d+)" path="([^"]+)" crc="([0-9a-fA-F]+)"', txt)
    print("flist entries:", len(ents))
    bad, missing, n = [], [], 0
    for size, path, crcval in ents:
        local = os.path.join(ROOT, path.replace("/", os.sep))
        if not os.path.exists(local):
            # windows is case insensitive
            cand = [p for p in (path.replace("/", "\\"),) if os.path.exists(os.path.join(ROOT, p))]
            if not cand:
                missing.append(path)
                continue
            local = os.path.join(ROOT, cand[0])
        n += 1
        sz = os.path.getsize(local)
        if sz != int(size):
            bad.append((path, "size", sz, int(size)))
            continue
        c = file_crc(local)
        if c != int(crcval, 16):
            bad.append((path, "crc", "%08x" % c, crcval))
    print("checked:", n, "missing:", len(missing), "bad:", len(bad))
    for m in missing[:20]:
        print("  MISSING", m)
    for b in bad[:30]:
        print("  BAD     %-40s %-5s local=%s flist=%s" % b)


if __name__ == "__main__":
    main()
