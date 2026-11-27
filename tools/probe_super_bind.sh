#!/bin/bash
# probe_super_bind.sh —— 单变量实验：干净环境下只起原生 SuperServer，定位 10000 bind 失败的真因
#
# 背景：tools/native_trial.sh 两次整栈实跑，21:28 成功、22:04 失败，同一二进制同一配置。
#       失败日志停在 zTCPServer::bind -> "不能绑定服务器端口"，但 zTCPServer::bind 不打印 errno。
#       本脚本把变量逐个钉死：先看 10000 的占用者，再彻底清场，再单独起 Super。
set -u
export WINEPREFIX=/root/.wine32
export WINEARCH=win32
export DISPLAY=:99
export WINEDEBUG=-all
export PATH=/opt/wine32/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin

NB=/root/p3-test/p3-934bf0d.vl0cYiyD/zhengtugf-p3-934bf0d/build/p3-native
SRV=/root/zt/server
LOGD=/root/zt/log/native
mkdir -p "$LOGD"

echo "=== 1. 清场前：谁占了 10000 / 8001 ==="
ss -lntp 2>/dev/null | grep -E ':(10000|8001) ' || echo "  none"

echo "=== 2. 清场 ==="
pkill -9 -f 'Server_r\.exe' 2>/dev/null
pkill -9 -f "$NB/" 2>/dev/null
sleep 2
wineserver -k 2>/dev/null
sleep 3

echo "=== 3. 清场后 ==="
ss -lntp 2>/dev/null | grep ':10000 ' || echo "  10000 free"
pgrep -af "$NB/" || echo "  no native proc"

echo "=== 4. MySQL 4.0 ==="
if ! pgrep -f mysqld >/dev/null; then bash /root/start_mysql40.sh >/dev/null 2>&1; fi
sleep 3
echo "  3307 listening=$(ss -lnt 2>/dev/null | grep -c ':3307 ')"

echo "=== 5. 只起原生 SuperServer ==="
: > "$LOGD/Super-probe.out"
cd "$SRV" && nohup "$NB/SuperServer" >> "$LOGD/Super-probe.out" 2>&1 &
SP=$!
sleep 6
if kill -0 "$SP" 2>/dev/null; then echo "  pid=$SP alive=yes"; else echo "  pid=$SP alive=no"; fi
ss -lntp 2>/dev/null | grep ':10000 ' || echo "  10000 NOT listening"

echo "=== 6. Super 日志尾部 ==="
iconv -f GB18030 -t UTF-8 "$LOGD/Super-probe.out" 2>/dev/null | tail -25

echo "=== 7. 用 python 直接复现 bind 拿 errno ==="
python3 - <<'PY'
import socket, errno
for fam, name in ((socket.AF_INET, "IPv4"),):
    s = socket.socket(fam, socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    try:
        s.bind(("0.0.0.0", 10000))
        s.listen(2000)
        print("  %s bind+listen 10000 OK" % name)
    except OSError as e:
        print("  %s bind 10000 FAIL errno=%d (%s)" % (name, e.errno, errno.errorcode.get(e.errno)))
    finally:
        s.close()
PY

echo "=== 8. 终态 ==="
ss -lntp 2>/dev/null | grep ':10000 ' || echo "  10000 NOT listening"
