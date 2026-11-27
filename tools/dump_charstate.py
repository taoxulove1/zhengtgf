import sys, struct
sys.path.insert(0, r"E:\源端\zhengtugf\tools")
import pak_extract

data = open(r"E:\源端\tmp-extract\characterstatebase.tbl", "rb").read()
n = struct.unpack_from("<I", data, 0)[0]
stride = (len(data) - 4) // n
print("count=%d stride=%d len=%d  (4+%d*%d=%d)" % (n, stride, len(data), n, stride, 4 + n * stride))
rec = struct.Struct("<I32sIIIII512sI")
print("packed struct size:", rec.size)
for i in range(6):
    off = 4 + i * stride
    dwID, name, p1, p2, p3, p4, st, desc, typ = rec.unpack_from(data, off)
    print("  #%d id=%-5d name=%-20r pic=%d,%d,%d,%d state=%d type=%d desc=%r"
          % (i, dwID, name.split(b"\0")[0].decode("gbk", "replace"),
             p1, p2, p3, p4, st, typ, desc.split(b"\0")[0][:40]))
ids = [struct.unpack_from("<I", data, 4 + i * stride)[0] for i in range(n)]
print("id range:", min(ids), max(ids), "unique:", len(set(ids)))
print("sorted:", ids == sorted(ids))
print("zeros:", ids.count(0))
print("tail bytes:", data[-8:].hex())
