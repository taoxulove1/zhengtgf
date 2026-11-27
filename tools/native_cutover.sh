#!/bin/bash
# native_cutover.sh —— 把客户端接入面从「Wine 托管原版 FL」切到「原生 FLServer」
#
# 背景（实测）：原版 FLServer_r.exe 的 8002 连上即 FIN，8000 收到 0x1E 就断，
#              客户端接入面在 Wine 下不可用；原生 FLServer（epoll 版）能正常应答。
# 做法：停全部 wine → 起 MySQL4.0 → 起原生 FLServer(8000/8001/8002) → 起其余原版服务 → 点启动 → 核对
set -u
export WINEPREFIX=/root/.wine32
export WINEARCH=win32
export DISPLAY=:99
export PATH=/opt/wine32/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
export WINEDEBUG=-all

NB=/root/p3-test/p3-934bf0d.vl0cYiyD/zhengtugf-p3-934bf0d/build/p3-native/FLServer
SRV=/root/zt/server

echo "########## 1. 停全部 wine ##########"
pkill -9 -f 'Server_r\.exe' 2>/dev/null
sleep 2
wineserver -k 2>/dev/null
sleep 3
echo "残留 exe: $(pgrep -fc '\.exe' || true)"

echo "########## 2. MySQL 4.0 ##########"
bash /root/start_mysql40.sh 2>&1 | tail -8

echo "########## 3. 原生 FLServer ##########"
cd "$SRV" || exit 1
nohup "$NB" > /root/zt/log/fl_native.out 2>&1 &
FLPID=$!
echo "native fl pid=$FLPID"
sleep 8
ss -lntp 2>/dev/null | grep -E ':(8000|8001|8002)\b' || echo "原生 FL 未监听"

echo "########## 4. 其余原版服务 ##########"
for n in super record bill session gate mini; do
  bash /root/zt/bin/srv.sh start "$n" 2>&1 | tail -1
  sleep 3
done

echo "########## 5. 点启动 ##########"
for cls in SuperdServer RecordServer BillServer SessionServer GatewayServer MiniServer; do
  W=$(xdotool search --name "$cls" 2>/dev/null | head -1)
  [ -z "$W" ] && { echo "-- $cls 无窗口"; continue; }
  xdotool windowraise "$W" 2>/dev/null; xdotool windowactivate "$W" 2>/dev/null; sleep 1
  xdotool mousemove --window "$W" 11 36 click 1
  echo "-- $cls clicked"
  sleep 5
done

echo "########## 6. 端口终态 ##########"
ss -lntp 2>/dev/null | grep -E ':(10000|8000|8001|8002|6001|6010|6012|6020|7010|7020|3307)\b'
echo "########## 7. 原生 FL 存活 ##########"
kill -0 "$FLPID" 2>/dev/null && echo "native fl alive pid=$FLPID" || echo "native fl DEAD"
