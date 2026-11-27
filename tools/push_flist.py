# -*- coding: utf-8 -*-
"""Push the local flist.xml (and optionally data/datas.pak.co) to the patch server."""
import os, sys, zlib, base64
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from remote import client, run as rrun

CLIENT = r"E:\源端\源1"
PREFIXES = ["/root/zt/www/autopatch/", "/root/zt/www/autopatchhuaijiu/",
            "/root/zt/www/Zhengtu/Client/autopatchhuaijiu/"]


def deflate_bytes(data):
    co = zlib.compressobj(6, zlib.DEFLATED, -15)
    return co.compress(data) + co.flush()


def put(c, remote_path, data):
    b64 = base64.b64encode(data).decode()
    si, so, se = c.exec_command("base64 -d > '%s'" % remote_path, timeout=180)
    step = 32768
    for i in range(0, len(b64), step):
        si.write(b64[i:i + step])
        si.flush()
    si.channel.shutdown_write()
    so.read()
    out, err, rc = rrun(c, "ls -l '%s' | awk '{print $5}'" % remote_path)
    return out.strip()


def main():
    xml = open(os.path.join(CLIENT, "flist.xml"), "rb").read()
    co = deflate_bytes(xml)
    back = zlib.decompressobj(-15).decompress(co)
    assert back == xml, "inflate mismatch"
    print("flist.xml=%d  co=%d  inflate-ok" % (len(xml), len(co)))

    c = client()
    for pref in PREFIXES:
        for name, data in (("flist.xml.co", co), ("srvflist.xml.co", co),
                           ("flist.xml", xml)):
            n = put(c, pref + name, data)
            print("  %-52s %s bytes" % (pref + name, n))
    c.close()


if __name__ == "__main__":
    main()
