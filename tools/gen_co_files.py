# -*- coding: utf-8 -*-
"""批量生成 .co 补丁文件（raw deflate，无加密）并上传服务器三路径。

.co 格式（已破解，20261128）：
    patchupdate 下载的 .co = raw deflate（windowBits=-15），无 zlib 头、无加密。
    验证：official-flist.xml.co inflate == official-flist.xml（逐字节一致）。
"""
import os
import zlib
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from remote import client, run as rrun

CLIENT = r"E:\源端\源1"
BACKUP = r"E:\源端\backup\dclient-launchers-20261128"


def deflate(path):
    co = zlib.compressobj(6, zlib.DEFLATED, -15)
    with open(path, "rb") as f:
        return co.compress(f.read()) + co.flush()


def deflate_bytes(data):
    co = zlib.compressobj(6, zlib.DEFLATED, -15)
    return co.compress(data) + co.flush()


def main():
    items = []  # (server_relpath, bytes)

    def add(rel, src_path, note=""):
        with open(src_path, "rb") as f:
            items.append((rel, deflate(src_path)))
        print("  %-28s <- %s (%d bytes raw) %s" % (rel, src_path, os.path.getsize(src_path), note))

    root = CLIENT
    add("flist.xml.co", os.path.join(root, "flist.xml"))
    add("srvflist.xml.co", os.path.join(root, "flist.xml"))
    add("patchsetup.ini.co", os.path.join(root, "patchsetup.ini"))
    add("patchupdate.exe.co", os.path.join(root, "patchupdate.exe"))
    add("config.ini.co", os.path.join(root, "config.ini"))
    # uninst.exe：客户端没有，用备份里的第三方 uninst.exe
    add("uninst.exe.co", os.path.join(BACKUP, "uninst.exe"))
    # ztsafe.sys：客户端没有，用官方样本解出的 8976B MZ
    zt = zlib.decompressobj(-15).decompress(
        open("captures/official-ztsafe.sys.co", "rb").read())
    items.append(("Data/ztsafe.sys.co", deflate_bytes(zt)))
    items.append(("data/ztsafe.sys.co", deflate_bytes(zt)))
    print("  Data/ztsafe.sys.co           <- official inflate (%d bytes)" % len(zt))

    epay = os.path.join(root, "epay")
    for fn in ("autoupdate.exe", "epayupdate.exe", "cfg.ini", "epay.exe",
               "unzip.exe", "autoupdate.xml", "Register.exe"):
        add("epay/" + fn + ".co", os.path.join(epay, fn))
    # register.exe 小写变体（Windows 大小写不敏感，Linux 服务器敏感）
    items.append(("epay/register.exe.co", deflate_bytes(
        open(os.path.join(epay, "Register.exe"), "rb").read())))
    # version.xml：服务器上的副本
    items.append(("epay/version.xml.co", None))  # 占位，下面从服务器拉

    # patchupdate.log stub
    items = [(r, b if b is not None else deflate_bytes(b"[log]\r\n")) if False else (r, b)
             for r, b in items]

    # 服务器拉取 version.xml / patchupdate.log 源
    c = client()
    out, err, rc = rrun(c, "cat /root/zt/www/autopatch/epay/version.xml 2>/dev/null")
    if not out.strip():
        out = '<?xml version="1.0" encoding="gb2312"?>\n<new>\n<cur>1.0.0.0</cur>\n</new>\n'
    vx = out.encode("gbk", "replace") if isinstance(out, str) else out
    items = [(r, deflate_bytes(vx) if r == "epay/version.xml.co" else b)
             for r, b in items]
    items = [(r, deflate_bytes(b"# patchupdate log stub\r\n") if r == "patchupdate.log.co" else b)
             for r, b in items]

    # 上传到三路径
    prefixes = ["/root/zt/www/autopatch/", "/root/zt/www/autopatchhuaijiu/",
                "/root/zt/www/Zhengtu/Client/autopatchhuaijiu/"]
    import base64
    for rel, data in items:
        for pref in prefixes:
            rpath = pref + rel
            rdir = os.path.dirname(rpath)
            rrun(c, "mkdir -p '%s'" % rdir)
            b64 = base64.b64encode(data).decode()
            cmd = "echo '%s' | base64 -d > '%s'" % (b64, rpath)
            # 大文件走分块 stdin 避免命令行超长
            if len(b64) > 100000:
                si, so, se = c.exec_command("base64 -d > '%s'" % rpath, timeout=120)
                step = 32768
                for i in range(0, len(b64), step):
                    si.write(b64[i:i + step])
                    si.flush()
                si.channel.shutdown_write()
                so.read()
            else:
                out, err, rc = rrun(c, cmd, 60)
                if rc != 0:
                    print("FAIL", rpath, err[:200])
                    continue
        # 校验
        out, err, rc = rrun(c, "ls -l '%s' | awk '{print $5}'" % (prefixes[0] + rel))
        print("  uploaded %-28s %s bytes" % (rel, out.strip()))

    # 清理客户端残留 .dl
    import glob
    n = 0
    for pat in (os.path.join(root, "*.dl"), os.path.join(root, "epay", "*.dl"),
                os.path.join(root, "Data", "*.dl"), os.path.join(root, "data", "*.dl")):
        for p in glob.glob(pat):
            if os.path.getsize(p) == 0:
                os.remove(p)
                n += 1
    print("removed %d zero-byte .dl residue" % n)


if __name__ == "__main__":
    main()
