import sys, os, shutil
sys.path.insert(0, r"E:\源端\zhengtugf\tools")
import pak_repack
from pak_variant_probe import run_client, PAK, BACKUP, TMP

NAME = "datas/tables/characterstatebase.tbl"


def run(label, rep=None, pad=0):
    dst = os.path.join(TMP, "p-%s.pak" % label)
    pak_repack.repack(BACKUP, dst, rep or {}, verbose=False, pad_last=pad)
    size = os.path.getsize(dst)
    shutil.copyfile(dst, PAK)
    alive, lines = run_client()
    msgs = [l.strip() for l in lines if "CHILD[Static]" in l or "EXITED" in l]
    print("== %-22s paksize=%d (+%d) alive=%s" % (label, size, size - 3135764, alive))
    for m in msgs:
        print("     ", m)
    return lines


if __name__ == "__main__":
    try:
        run("pristine+8", pad=8)
    finally:
        shutil.copyfile(BACKUP, PAK)
        print("restored")
