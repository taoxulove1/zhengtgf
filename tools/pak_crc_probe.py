import sys, os, struct
sys.path.insert(0, r"E:\源端\zhengtugf\tools")
import des_mydes as DES
import pak_open

T = []
for i in range(256):
    v = i
    for _ in range(8):
        v = (0xEDB88320 ^ (v >> 1)) if v & 1 else v >> 1
    T.append(v & 0xFFFFFFFF)


def crc(buf, seed=0xFFFFFFFF):
    c = seed
    for b in buf:
        c = T[(c ^ b) & 0xFF] ^ (c >> 8)
    return c & 0xFFFFFFFF


names = [
    "datas/tables/characterstatebase.tbl",
    "datas\\tables\\characterstatebase.tbl",
    "datas/tables/CharacterStateBase.tbl",
    "datas\\tables\\CharacterStateBase.tbl",
    "datas\\tables\\CharacterStateBase.tbl".lower(),
]
print("target id = 2076979135 (0x%08x)" % 2076979135)
for n in names:
    v = crc(n.encode())
    print("  %-45s -> %10d 0x%08x %s" % (n, v, v, "MATCH" if v == 2076979135 else ""))

# verify payload crc chain for the extracted entry
pak = r"E:\源端\源1\Data\datas.pak"
data = open(pak, "rb").read()
info = pak_open.read_pak(pak, b"xuzhao20041108")
ent = [e for e in info["entries"] if e["name"] == "datas/tables/characterstatebase.tbl"][0]
base = 20 + ent["off"]
raw = data[base:base + ent["size"]]
c = crc(raw[:16])
c2 = crc(raw[16:], c)
print("entry crc chain: hdr=%08x payload_seeded=%08x stored=%08x %s"
      % (crc(raw[:16]), c2, ent["crc"], "MATCH" if c2 == ent["crc"] else "MISMATCH"))
