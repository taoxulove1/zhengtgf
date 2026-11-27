#!/bin/bash
# scenes_verbose.sh —— 用 zh_CN.GBK locale 跑原生 ScenesServer，让 log4cxx 的中文日志可读
#
# 背景：前几轮日志里中文全部变成字面 '?'，是 log4cxx 在 C locale 下做窄字符转码造成的，
#       导致 "初始化基本数据模块...失败" 这类关键报错读不出来。
#       已用 localedef 生成 zh_CN.GBK（glibc 自带 GBK charmap + zh_CN locale 源）。
#       源码窄字符串本来就是 GBK 编码，locale 设成 GBK 后原样落盘，iconv 一下即可读。
set -u
export WINEPREFIX=/root/.wine32 WINEARCH=win32 DISPLAY=:99 WINEDEBUG=-all
export PATH=/opt/wine32/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
export LC_ALL=zh_CN.GBK
export LANG=zh_CN.GBK

NB=/root/p3-test/p3-934bf0d.vl0cYiyD/zhengtugf-p3-934bf0d/build/p3-native
LOG=/root/zt/log/native/ScenesServer.out
mkdir -p /root/zt/server/log/nat

cd /root/zt/server || exit 1
: > "$LOG"
nohup "$NB/ScenesServer" >> "$LOG" 2>&1 &
SP=$!
sleep 10
echo "pid=$SP alive=$(kill -0 $SP 2>/dev/null && echo yes || echo no)"
ss -lnt 2>/dev/null | grep ':6010 ' || echo "  6010 NOT listening"

echo "===== ERROR/FATAL（GBK 转码） ====="
iconv -f GBK -t UTF-8 "$LOG" 2>/dev/null | grep -aE 'ERROR|FATAL' | cut -c1-180 | head -30
echo "===== 尾部 ====="
iconv -f GBK -t UTF-8 "$LOG" 2>/dev/null | tail -10 | cut -c1-180
