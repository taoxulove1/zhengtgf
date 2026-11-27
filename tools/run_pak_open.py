import sys, os
sys.path.insert(0, r"E:\源端\zhengtugf\tools")
import pak_open

pak = r"E:\源端\源1\Data\datas.pak"
print("pak:", pak, os.path.getsize(pak))
best = pak_open.open_best(pak)
if best:
    s, pw, info = best
    print("best pw=%s ratio=%.3f version=%d files=%d hdr=%d flag=0x%x consumed=%d"
          % (pw, s, info["version"], info["filenum"], info["headerSize"],
             info["flag"], info["consumed"]))
    hits = [e for e in info["entries"] if "characterstate" in e["name"].lower()]
    print("charstate hits:", len(hits))
    for e in hits:
        print("   id=%d off=%d size=%d crc=%08x %s"
              % (e["id"], e["off"], e["size"], e["crc"], e["name"]))
    print("first 15 names:")
    for e in info["entries"][:15]:
        print("   ", e["name"], e["size"])
