#!/bin/bash
# stack_restart.sh —— 原版服务端全栈干净重启（供采「有效回包」用）
# 顺序：停全部 wine → 起 MySQL4.0(3307) → srv.sh boot → second_pass 点启动 → 端口核对
set -u
export WINEPREFIX=/root/.wine32
export WINEARCH=win32
export DISPLAY=:99
export PATH=/opt/wine32/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
export WINEDEBUG=-all

echo "########## 1. 停全部 ##########"
bash /root/zt/bin/srv.sh stopall 2>&1 | tail -10
pkill -f ScenesServer_r.exe 2>/dev/null
sleep 2
wineserver -k 2>/dev/null
sleep 3
echo "剩余 wine 进程: $(pgrep -f '\.exe' | wc -l)"

echo "########## 2. 起 MySQL 4.0 ##########"
bash /root/start_mysql40.sh 2>&1 | tail -18

echo "########## 3. srv.sh boot ##########"
bash /root/zt/bin/srv.sh boot 2>&1 | tail -14

echo "########## 4. 点启动 ##########"
bash /root/second_pass.sh 2>&1 | grep -E '^(===|   log|#####)' | tail -20

echo "########## 5. 端口终态 ##########"
ss -lntp 2>/dev/null | grep -E ':(10000|8000|8001|8002|6001|6010|6012|6020|7010|7020|9967|3307)\b' || echo "无端口"
echo "########## 6. 进程 ##########"
ps aux | grep -E '[A-Za-z]+Server_r\.exe|mysqld\.exe' | grep -v grep | awk '{print $2, $11}'
