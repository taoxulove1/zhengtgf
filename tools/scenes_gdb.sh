#!/bin/bash
# scenes_gdb.sh —— 用 gdb 抓原生 ScenesServer 初始化期 segfault 的调用栈
#
# 现状（tools/native_boot.sh 实测）：整栈原生已走到
#   ScenesServer  INFO: 初始化 场景服务器:6010 成功   -> 6010 可以 bind
#   紧接着加载 NPC 数据报大量 ERROR，然后 Segmentation fault 退出，6010 随之消失。
# 所以「6010 起不来」的真正断点是 Scenes 初始化期的空指针，不是端口、不是配置。
set -u
export WINEPREFIX=/root/.wine32 WINEARCH=win32 DISPLAY=:99 WINEDEBUG=-all
export PATH=/opt/wine32/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin

NB=/root/p3-test/p3-934bf0d.vl0cYiyD/zhengtugf-p3-934bf0d/build/p3-native
SRV=/root/zt/server
OUT=/root/zt/log/native/scenes-gdb.txt
mkdir -p /root/zt/server/log/nat

cd "$SRV" || exit 1
gdb -batch -nx \
    -ex 'set pagination off' \
    -ex 'set confirm off' \
    -ex 'handle SIGPIPE nostop noprint pass' \
    -ex 'run' \
    -ex 'echo \n===== BACKTRACE =====\n' \
    -ex 'bt 40' \
    -ex 'echo \n===== FRAME 0 DETAIL =====\n' \
    -ex 'info registers' \
    -ex 'echo \n===== THREADS =====\n' \
    -ex 'info threads' \
    -ex 'echo \n===== LOCALS =====\n' \
    -ex 'frame 0' -ex 'info args' -ex 'info locals' \
    --args "$NB/ScenesServer" > "$OUT" 2>&1
echo "gdb exit=$?  size=$(wc -c < "$OUT")"
