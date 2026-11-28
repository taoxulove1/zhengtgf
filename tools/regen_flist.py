# -*- coding: utf-8 -*-
"""按官方 flist 收录口径（captures/official-flist.xml, 716条）重新生成 flist：

官方不收录：flist.xml / srvflist* / config.ini / uninst* / ztsafe* / epay/ 全部 /
            *.log / *.bak* / *.tmp* / *.dl / data/log 目录 / 无扩展名工作文件
官方收录：  patchsetup.ini、patchupdate.exe、游戏数据件（.grp/.pak/.dll/.exe 等）

产出：flist.xml → flist.xml.co → 上传三路径；同步替换客户端本地 flist.xml。
CRC 算法：zlib 多项式 init=0xFFFFFFFF 无 final xor（已破解验证 94/95）。
"""
import os
import sys
import zlib

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from remote import client, run as rrun

SRC = r"E:\源端\源1"


def crcfile(p):
    c = 0
    with open(p, "rb") as f:
        for b in iter(lambda: f.read(1 << 20), b""):
            c = zlib.crc32(b, c)
    return (c & 0xFFFFFFFF) ^ 0xFFFFFFFF


def excluded(rel_lower):
    parts = rel_lower.split("/")
    name = parts[-1]
    # err.txt：登录器每次启动追加一行时间戳（21 字节），纳入 flist 必然引发
    # "size 不符 → 请求 err.txt.co → 404 → 游戏更新失败"，必须永久排除。
    if name in ("flist.xml", "srvflist.xml", "config.ini", "flist", "patchupdate.log", "err.txt"):
        return "self/tool"
    if name.endswith((".pristine", ".orig", ".orig-bak", ".noLAA.bak")):
        return "backup-copy"
    if name.startswith("uninst") or name.startswith("ztsafe"):
        return "uninst/ztsafe"
    if name.endswith((".log", ".bak", ".tmp", ".tmp2", ".dl")) or ".bak-" in name:
        return "log/bak/tmp/dl"
    if parts[0] == "epay":
        return "epay"
    if parts[0] == "data" and len(parts) > 1 and parts[1] == "log":
        return "data/log"
    if name.endswith(".dism") or name.endswith(".tmp2"):
        return "tmp"
    return None


def main():
    lines = ['<?xml version="1.0" encoding="GB2312" ?>', "<files>"]
    n = 0
    skipped = {}
    for root, dirs, files in os.walk(SRC):
        dirs[:] = [d for d in dirs if d.lower() not in ("log",)]
        for fn in sorted(files):
            full = os.path.join(root, fn)
            rel = os.path.relpath(full, SRC).replace("\\", "/").lower()
            why = excluded(rel)
            if why:
                skipped[why] = skipped.get(why, 0) + 1
                continue
            try:
                crc = crcfile(full)
            except OSError:
                continue
            lines.append('    <file size="%d" path="%s" crc="%x" must="1" />' % (
                os.path.getsize(full), rel, crc))
            n += 1
    lines.append("</files>")
    xml = "\r\n".join(lines)
    print("kept files:", n)
    print("skipped:", skipped)
    xml_bytes = xml.encode("gbk")

    # 写客户端本地 flist.xml（GBK，与 patchupdate 编码一致）
    local = os.path.join(SRC, "flist.xml")
    with open(local, "wb") as f:
        f.write(xml_bytes)
    print("local flist.xml:", len(xml_bytes), "bytes")

    # .co
    co = zlib.compressobj(6, zlib.DEFLATED, -15)
    co_data = co.compress(xml_bytes) + co.flush()
    print("flist.xml.co:", len(co_data), "bytes")

    # 自验：inflate 回验
    back = zlib.decompressobj(-15).decompress(co_data)
    assert back == xml_bytes, "inflate mismatch!"
    print("inflate-back: OK", len(back), "bytes")

    # 上传三路径（flist.xml.co + srvflist.xml.co 同内容 + 明文 flist.xml）
    import base64
    b64 = base64.b64encode(co_data).decode()
    b64xml = base64.b64encode(xml_bytes).decode()
    c = client()
    prefixes = ["/root/zt/www/autopatch/", "/root/zt/www/autopatchhuaijiu/",
                "/root/zt/www/Zhengtu/Client/autopatchhuaijiu/"]
    for pref in prefixes:
        si, so, se = c.exec_command("base64 -d > '%sflist.xml.co'" % pref, timeout=60)
        step = 32768
        for i in range(0, len(b64), step):
            si.write(b64[i:i + step])
        si.channel.shutdown_write()
        so.read()
        si, so, se = c.exec_command("base64 -d > '%ssrvflist.xml.co'" % pref, timeout=60)
        for i in range(0, len(b64), step):
            si.write(b64[i:i + step])
        si.channel.shutdown_write()
        so.read()
        out, err, rc = rrun(c, "echo '%s' | base64 -d > '%sflist.xml'" % (b64xml, pref), 30)
        s1, _, _ = rrun(c, "ls -l '%sflist.xml.co' '%sflist.xml' | awk '{print $5}'" % (pref, pref))
        print(pref, "->", s1.strip().split())

    # 外网自验
    print("self-check inflates on server done")


if __name__ == "__main__":
    main()
