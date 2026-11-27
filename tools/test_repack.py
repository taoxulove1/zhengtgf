import sys, os, struct
sys.path.insert(0, r"E:\源端\zhengtugf\tools")
import pak_open, pak_repack

SRC = r"E:\源端\源1\Data\datas.pak"
DST = r"E:\源端\tmp-paktest\datas-repack.pak"
os.makedirs(os.path.dirname(DST), exist_ok=True)

print("[1] identical repack")
pak_repack.repack(SRC, DST)
a = pak_open.read_pak(SRC, b"xuzhao20041108")
b = pak_open.read_pak(DST, b"xuzhao20041108")
print("  src files=%d flag=%d hdr=%d" % (a["filenum"], a["flag"], a["headerSize"]))
print("  dst files=%d flag=%d hdr=%d" % (b["filenum"], b["flag"], b["headerSize"]))
same = all(x["name"] == y["name"] and x["off"] == y["off"] and x["size"] == y["size"]
           and x["crc"] == y["crc"] and x["id"] == y["id"]
           for x, y in zip(a["entries"], b["entries"]))
print("  entry metadata identical:", same)
print("  sizes: %d -> %d" % (os.path.getsize(SRC), os.path.getsize(DST)))

# extract one entry from the rebuilt pak and compare bytes with the original
import pak_extract
o1 = pak_extract.extract(SRC, "datas/tables/characterstatebase.tbl", r"E:\源端\tmp-paktest\a")
o2 = pak_extract.extract(DST, "datas/tables/characterstatebase.tbl", r"E:\源端\tmp-paktest\b")
print("  payload identical:", o1 == o2, len(o1))
