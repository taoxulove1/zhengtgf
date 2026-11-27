"""Install a rebuilt datas.pak into the client and keep Data's flist.xml in sync.

The launcher/client verifies every file in flist.xml by (size, crc) using the
engine CRC (init 0xffffffff, no final xor).  Changing datas.pak therefore also
requires rewriting that one entry, otherwise the client reports
"Data\\Datas.pak 文件不是最新版本或已损坏".
"""
import sys, os, re, shutil, datetime
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import pak_repack

ROOT = r"E:\源端\源1"
PAK = os.path.join(ROOT, "Data", "datas.pak")
FLIST = os.path.join(ROOT, "flist.xml")


def engine_crc_file(path, chunk=1 << 20):
    c = 0xFFFFFFFF
    with open(path, "rb") as f:
        while True:
            b = f.read(chunk)
            if not b:
                break
            c = pak_repack.crc(b, c)
    return c & 0xFFFFFFFF


def filetime_parts(path):
    ts = os.path.getmtime(path)
    ft = int((ts + 11644473600) * 10000000)
    return ft & 0xFFFFFFFF, (ft >> 32) & 0xFFFFFFFF


def patch_flist(path, size, crc, low, high, verbose=True):
    with open(path, "r", encoding="gb2312", errors="surrogateescape", newline="") as f:
        txt = f.read()
    pat = re.compile(r'<file size="\d+" path="data/datas\.pak" crc="[0-9a-fA-F]+"[^/]*/>')
    m = pat.search(txt)
    if not m:
        raise RuntimeError("no data/datas.pak entry in flist.xml")
    old = m.group(0)
    new = ('<file size="%d" path="data/datas.pak" crc="%08x" must="1" '
           'lowdatetime="%x" highdatetime="%x" />' % (size, crc, low, high))
    txt = txt[:m.start()] + new + txt[m.end():]
    with open(path, "wb") as f:
        f.write(txt.encode("gb2312", "surrogateescape"))
    if verbose:
        print("  flist  old:", old)
        print("  flist  new:", new)


def install(pak_path, verbose=True):
    shutil.copyfile(pak_path, PAK)
    size = os.path.getsize(PAK)
    crc = engine_crc_file(PAK)
    low, high = filetime_parts(PAK)
    if verbose:
        print("  installed %s size=%d crc=%08x ft=%x:%x" % (PAK, size, crc, low, high))
    patch_flist(FLIST, size, crc, low, high, verbose)


if __name__ == "__main__":
    install(sys.argv[1] if len(sys.argv) > 1 else r"E:\源端\tmp-paktest\real576.pak")
