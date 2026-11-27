#!/bin/bash
# scene_probe.sh —— 在服务器的 Xvfb 上启动 ScenesServer，抓窗口列表 + 截图，供人工判断弹窗内容
set -u
export WINEPREFIX=/root/.wine32
export DISPLAY=:99
export LANG=C
export LC_ALL=C
export WINEDEBUG=-all
cd /root/zt/server || exit 1

pkill -f ScenesServer_r.exe 2>/dev/null
sleep 1

nohup /opt/wine32/bin/wine ScenesServer_r.exe > /tmp/scene_run.txt 2>&1 &
echo "started pid=$!"
sleep 8

echo "=== alive ==="
ps aux | grep -c "[S]cenesServer_r.exe"
echo "=== windows ==="
xdotool search --onlyvisible --name "" 2>/dev/null | while read -r w; do
  printf "%s | %s\n" "$w" "$(xdotool getwindowname "$w" 2>/dev/null)"
done
echo "=== port 6010 ==="
ss -lntp 2>/dev/null | grep 6010 || echo "6010 未监听"
echo "=== screenshot ==="
command -v import >/dev/null && import -window root /tmp/scene_shot.png && echo "/tmp/scene_shot.png $(stat -c%s /tmp/scene_shot.png) bytes"
echo "=== app log tail ==="
ls -t /root/zt/server/*.log 2>/dev/null | head -2 | while read -r f; do echo "-- $f"; tail -12 "$f"; done
