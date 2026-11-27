"""Probe the record stride the 2026 client expects for CharacterStateBase.tbl.

Baseline pak loads 940 records x 572 bytes and the client says "数据有误".
With 0 records it passes the check and crashes later -> the check is almost
certainly a short-read, i.e. the client's struct is bigger than 572 bytes.
Here we feed 940 records with a larger stride (ID at offset 0, zero elsewhere)
and see which stride gets past the table check.
"""
import sys, os, shutil
sys.path.insert(0, r"E:\源端\zhengtugf\tools")
import pak_repack
from pak_variant_probe import run_client, PAK, BACKUP, TMP, ROOT

NAME = "datas/tables/characterstatebase.tbl"


def build(stride, count=940):
    buf = bytearray()
    buf += count.to_bytes(4, "little")
    for i in range(count):
        rec = bytearray(stride)
        rec[0:4] = (i * 2 + 1).to_bytes(4, "little")
        buf += rec
    return bytes(buf)


def one(label, data):
    dst = os.path.join(TMP, "s-%s.pak" % label)
    pak_repack.repack(BACKUP, dst, {NAME: data}, verbose=False)
    shutil.copyfile(dst, PAK)
    alive, lines = run_client()
    msg = [l for l in lines if "CHILD[Static]" in l or "EXITED" in l]
    print("== stride %-6s size=%-8d alive=%s" % (label, len(data), alive))
    for m in msg:
        print("     ", m.strip())
    return alive, lines


if __name__ == "__main__":
    strides = [int(x) for x in sys.argv[1:]] or [576, 604, 640, 768, 1024]
    try:
        for s in strides:
            one(str(s), build(s))
    finally:
        shutil.copyfile(BACKUP, PAK)
        print("restored")
