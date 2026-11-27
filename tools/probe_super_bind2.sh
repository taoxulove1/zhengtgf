#!/bin/bash
# probe_super_bind2.sh —— 第二轮单变量：ifname 改成网卡名 eth0 后，单起原生 SuperServer
#
# 第一轮(tools/probe_super_bind.sh)已排除：端口占用、MySQL、残留进程。
# 第一轮日志给出两条独立故障：
#   A) <ifname> 配的是 IP(10.1.0.10) -> getIPByIfName 走 ioctl(SIOCGIFADDR) 失败 -> "0.0.0.0"，
#      原生栈随后报「本机 IP 不正确 0.0.0.0, 10.1.0.10」直接退出（根本没走到 bind）。
#   B) 22:04 那次 ifname 已对，日志停在 zTCPServer::bind ->「不能绑定服务器端口」。
# 本脚本先修 A，看 B 是否复现；并把 bind 的 errno 用 python 单独测一遍。
set -u
export WINEPREFIX=/root/.wine32
export WINEARCH=win32
export DISPLAY=:99
export WINEDEBUG=-all
export PATH=/opt/wine32/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin

CFG=/root/zt/server/config.xml
NB=/root/p3-test/p3-934bf0d.vl0cYiyD/zhengtugf-p3-934bf0d/build/p3-native
LOGD=/root/zt/log/native
mkdir -p "$LOGD" /root/zt/server/log/nat

echo "=== 0. 前置：清场 ==="
pkill -9 -f 'Server_r\.exe' 2>/dev/null
pkill -9 -f "$NB/" 2>/dev/null
sleep 2
wineserver -k 2>/dev/null
sleep 3
ss -lntp 2>/dev/null | grep -E ':(10000|8001) ' || echo "  10000/8001 free"
pgrep -f mysqld >/dev/null || (bash /root/start_mysql40.sh >/dev/null 2>&1; sleep 3)
echo "  3307 listening=$(ss -lnt 2>/dev/null | grep -c ':3307 ')"

echo "=== 1. ifname -> eth0 / logfilename -> 文件 ==="
if [ ! -f "$CFG.bak-orig" ]; then cp "$CFG" "$CFG.bak-orig"; fi
sed -i 's#<ifname>10.1.0.10</ifname>#<ifname>eth0</ifname>#' "$CFG"
python3 - <<'PY'
import re
p = "/root/zt/server/config.xml"
d = open(p, "rb").read()
d2, n = re.subn(rb"<logfilename>log/([A-Za-z]+)</logfilename>", rb"<logfilename>log/nat/\1.log</logfilename>", d)
if n:
    open(p, "wb").write(d2)
print("  logfilename -> log/nat/*.log :", n)
PY
grep -c '<ifname>eth0</ifname>' "$CFG" | sed 's/^/  eth0 出现次数: /'

echo "=== 2. python 先单独试 bind 10000（拿 errno） ==="
python3 - <<'PY'
import socket, errno
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
try:
    s.bind(("0.0.0.0", 10000)); s.listen(2000)
    print("  bind+listen 10000 OK (端口是干净的)")
except OSError as e:
    print("  bind FAIL errno=%d (%s)" % (e.errno, errno.errorcode.get(e.errno)))
finally:
    s.close()
PY

echo "=== 3. 起原生 SuperServer ==="
: > "$LOGD/Super-probe2.out"
cd /root/zt/server && nohup "$NB/SuperServer" >> "$LOGD/Super-probe2.out" 2>&1 &
SP=$!
sleep 8
kill -0 "$SP" 2>/dev/null && echo "  pid=$SP alive=yes" || echo "  pid=$SP alive=no"
ss -lntp 2>/dev/null | grep ':10000 ' | sed 's/^/  /' || echo "  10000 NOT listening"

echo "=== 4. 日志 ==="
iconv -f GB18030 -t UTF-8 "$LOGD/Super-probe2.out" 2>/dev/null | tail -18
