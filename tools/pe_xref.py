"""Locate a GBK string in a PE and find code xrefs (imm32 operands) to it."""
import sys, struct, re

def load(path):
    d = open(path, "rb").read()
    e = struct.unpack_from("<I", d, 0x3C)[0]
    nsec = struct.unpack_from("<H", d, e + 6)[0]
    optsz = struct.unpack_from("<H", d, e + 20)[0]
    opt = e + 24
    imagebase = struct.unpack_from("<I", d, opt + 28)[0]
    secs = []
    o = opt + optsz
    for i in range(nsec):
        name = d[o:o + 8].rstrip(b"\0").decode("latin1")
        vsize, vaddr, rawsize, rawptr = struct.unpack_from("<IIII", d, o + 8)
        secs.append((name, vaddr, vsize, rawptr, rawsize))
        o += 40
    return d, imagebase, secs


def off2va(secs, imagebase, off):
    for name, va, vs, rp, rs in secs:
        if rp <= off < rp + rs:
            return imagebase + va + (off - rp), name
    return None, None


def va2off(secs, imagebase, va):
    rva = va - imagebase
    for name, sva, vs, rp, rs in secs:
        if sva <= rva < sva + max(vs, rs):
            return rp + (rva - sva), name
    return None, None


def main():
    path = sys.argv[1]
    needle = sys.argv[2].encode("gbk")
    d, ib, secs = load(path)
    print("imagebase=%08x sections=%s" % (ib, [s[0] for s in secs]))
    hits = [m.start() for m in re.finditer(re.escape(needle), d)]
    print("string hits:", len(hits))
    for h in hits[:6]:
        va, sec = off2va(secs, ib, h)
        print("  off=%08x va=%08x sec=%s ctx=%r" % (h, va, sec, d[max(0, h - 40):h + len(needle) + 40]))
        if va is None:
            continue
        pat = struct.pack("<I", va)
        refs = [m.start() for m in re.finditer(re.escape(pat), d)]
        print("    imm32 refs:", len(refs))
        for r in refs[:8]:
            rva, rsec = off2va(secs, ib, r)
            print("      ref off=%08x va=%08x sec=%s" % (r, rva, rsec))


if __name__ == "__main__":
    main()
