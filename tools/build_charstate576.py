"""Build a 576-byte-stride CharacterStateBase.tbl from the real 572-byte data."""
import sys, struct
sys.path.insert(0, r"E:\源端\zhengtugf\tools")

SRC = r"E:\源端\tmp-extract\characterstatebase.tbl"
REC572 = struct.Struct("<I32sIIIII512sI")


def build(path, extra=4, pad_at_end=True):
    data = open(SRC, "rb").read()
    n = struct.unpack_from("<I", data, 0)[0]
    out = bytearray(struct.pack("<I", n))
    for i in range(n):
        rec = data[4 + i * 572:4 + (i + 1) * 572]
        out += rec + b"\0" * extra
    open(path, "wb").write(out)
    print("built %s  count=%d stride=%d size=%d" % (path, n, 572 + extra, len(out)))
    return bytes(out)


if __name__ == "__main__":
    build(r"E:\源端\tmp-paktest\charstate-576.tbl")
