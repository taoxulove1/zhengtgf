# -*- coding: utf-8 -*-
"""生成官方登录器 HTTP 伺服全套：srvlist/autoupdate/flist/board/logo。

格式对照官方怀旧服在线样本 captures/official-flist.xml：
  - flist.xml: <files><file size=".." path=".." crc=".." must="1"/></files>，crc 为 %x 不补零，path 小写+反斜杠
  - srvlist.xml: gametype="7"，patch item type="101"，zone type="710"，GB2312
"""
import os
import zlib

SRC = r"E:\源端\源1"
OUT = r"E:\源端\zhengtugf\dist-www"
SRV_IP = "101.33.242.142"
ZONE = u"雲天征途"

os.makedirs(os.path.join(OUT, "autopatch"), exist_ok=True)


def crcfile(p):
    """官方 flist CRC 算法（官方怀旧样本 12/12 实测命中）：
    zlib 多项式 0xEDB88320，init=0xFFFFFFFF，**无 final xor**，
    即标准 zlib.crc32() 结果 ^ 0xFFFFFFFF，格式 %x 不补零。"""
    c = 0
    with open(p, "rb") as f:
        for b in iter(lambda: f.read(1 << 20), b""):
            c = zlib.crc32(b, c)
    return (c & 0xFFFFFFFF) ^ 0xFFFFFFFF


# 1) flist.xml
lines = ['<?xml version="1.0" encoding="GB2312" ?>', "<files>"]
n = 0
for root, dirs, files in os.walk(SRC):
    for fn in sorted(files):
        full = os.path.join(root, fn)
        rel = os.path.relpath(full, SRC).replace("\\", "/").lower()
        lines.append('    <file size="%d" path="%s" crc="%x" must="1" />' % (
            os.path.getsize(full), rel, crcfile(full)))
        n += 1
lines.append("</files>")
open(os.path.join(OUT, "autopatch", "flist.xml"), "w", newline="\r\n").write("\n".join(lines))
print("flist.xml files:", n)

# 2) srvlist.xml (GB2312)
srv = (u'<?xml version="1.0" encoding="GB2312"?>\n'
       u'<server gametype="7">\n'
       u'  <patch>\n'
       u'    <item addr="%s" port="80" path="/autopatch" type="101" reservepatch="%s"/>\n'
       u'  </patch>\n'
       u'  <zones default="%s">\n'
       u'    <zone name="%s" id="1" addr="%s" port="8000" type="710" zone="0" Index="1" newzone="1"/>\n'
       u'  </zones>\n'
       u'</server>\n') % (SRV_IP, SRV_IP, ZONE, ZONE, SRV_IP)
open(os.path.join(OUT, "autopatch", "srvlist.xml"), "wb").write(srv.encode("gbk"))  # 雲 不在 GB2312，用 GBK 超集（Windows 按 CP936 解码）

# 3) autoupdate.xml —— 空更新列表（本服无需补丁）
au = u'<?xml version="1.0" encoding="GB2312"?>\n<files>\n</files>\n'
open(os.path.join(OUT, "autopatch", "autoupdate.xml"), "wb").write(au.encode("gbk"))

# 4) board.htm / logo.shtml (GB2312)
page = (u'<html><head><meta http-equiv="Content-Type" content="text/html; charset=gb2312">'
        u'<title>%s公告</title></head>\n'
        u'<body bgcolor="#101828" text="#ffd700" link="#00ccff">\n'
        u'<center><h2>%s 公告</h2></center>\n'
        u'<font size="4">\n'
        u'<p>&middot; 经典怀旧 3.1.46.0 全新开放，官方登录器 patchupdate.exe 直接登录。</p>\n'
        u'<p>&middot; 登录地址：%s（登录端口 8000，网关 6020）。</p>\n'
        u'<p>&middot; 启动器提示更新时直接点「开始游戏」，本服无需补丁。</p>\n'
        u'</font>\n</body></html>\n') % (ZONE, ZONE, SRV_IP)
for name in ("board.htm", "logo.shtml"):
    open(os.path.join(OUT, "autopatch", name), "wb").write(page.encode("gbk"))

print("written to", OUT)
