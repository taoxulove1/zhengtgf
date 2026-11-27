#!/bin/bash
# native_boot.sh —— 整栈原生启动（修正版启动顺序）
#
# 相对 native_trial.sh 的三处修正，全部来自 tools/probe_super_bind.sh / probe_super_bind2.sh 的实测：
#
#   1) 启动顺序：FLServer 必须在 SuperServer 之前。
#      SuperService::init() 里 FLClientManager::init() 会建 FLClient(10.1.0.10:8001)，
#      其后 4s 首次 connect，连续失败两次就 finalHandle() 退出（22:04 日志为证）。
#      而 ScenesServer/RecordServer 等又要连 Super(10000)，Super 一死全链崩 -> 22:04 的 6010 缺席是连锁。
#
#   2) 故障 A（真因）已修：<ifname> 必须是网卡名 eth0，不能是 IP。
#      zSocket::getIPByIfName() 走 ioctl(SIOCGIFADDR)，传 "10.1.0.10" 必然失败返回 "0.0.0.0"，
#      原生栈随后报「本机 IP 不正确 0.0.0.0, 10.1.0.10」直接退出——根本没走到 bind。
#      （Wine 原版反过来只认 IP，所以两种栈不能共用一份 config.xml。）
#
#   3) 故障 B 不存在：端口是干净的。python 单测 bind(0.0.0.0:10000) OK，原生 Super 单起也 OK。
#      22:04 的「不能绑定服务器端口」是 A 的次生现象（当时配置处于半切换状态）。
#
# 判据：6010 监听 => 成功；否则打印每个服务的日志尾巴供定位，不做自动回滚（回滚见 native_trial.sh）。
set -u
export WINEPREFIX=/root/.wine32
export WINEARCH=win32
export DISPLAY=:99
export WINEDEBUG=-all
export PATH=/opt/wine32/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin

CFG=/root/zt/server/config.xml
SRV=/root/zt/server
NB=/root/p3-test/p3-934bf0d.vl0cYiyD/zhengtugf-p3-934bf0d/build/p3-native
LOGD=/root/zt/log/native
OUT=/root/zt/run
mkdir -p "$LOGD" "$OUT" "$SRV/log/nat"

# 顺序：FL 先，Super 次之，其余按依赖（Mini 必须先于 Gateway，见 GatewayServer.cpp:158）
ORDER="FLServer SuperServer MiniServer RecordServer BillServer SessionServer GatewayServer ScenesServer"
declare -A PORT=( [SuperServer]=10000 [RecordServer]=7010 [BillServer]=7020 [MiniServer]=6012
                  [FLServer]=8000 [SessionServer]=6001 [GatewayServer]=6020 [ScenesServer]=6010 )

echo "########## 0. 清场 ##########"
pkill -9 -f 'Server_r\.exe' 2>/dev/null
pkill -9 -f "$NB/" 2>/dev/null
sleep 2
wineserver -k 2>/dev/null
sleep 3
echo "  留存 native: $(pgrep -fc "$NB/" || echo 0)"
echo "  10000: $(ss -lnt 2>/dev/null | grep -c ':10000 ')  8000: $(ss -lnt 2>/dev/null | grep -c ':8000 ')"

echo "########## 1. 配置就位（ifname=eth0 / logfilename=文件 / threadPoolCapacity） ##########"
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
tp = d2.count(b"<threadPoolCapacity>")
print("  threadPoolCapacity 段数:", tp, "(需 8)")
PY

echo "########## 2. MySQL 4.0 ##########"
if ! pgrep -f mysqld >/dev/null; then bash /root/start_mysql40.sh > "$LOGD/mysql40.out" 2>&1; sleep 4; fi
echo "  3307 listening=$(ss -lnt 2>/dev/null | grep -c ':3307 ')"

echo "########## 3. 按序启动 ##########"
start_one() {
  n="$1"; wait_s="$2"
  bin="$NB/$n"; log="$LOGD/$n.out"
  if [ ! -x "$bin" ]; then echo "  [$n] 缺可执行文件，跳过"; return; fi
  : > "$log"
  ( cd "$SRV" && nohup "$bin" >> "$log" 2>&1 & echo $! > "$OUT/$n.pid" )
  sleep "$wait_s"
  pid=$(cat "$OUT/$n.pid" 2>/dev/null || echo "?")
  alive=no; kill -0 "$pid" 2>/dev/null && alive=yes
  listen=$(ss -lnt 2>/dev/null | grep -c ":${PORT[$n]} ")
  err=$(iconv -f GB18030 -t UTF-8 "$log" 2>/dev/null | grep -aE 'ERROR|FATAL' | tail -1 | cut -c1-140)
  echo "  [$n] pid=$pid alive=$alive :${PORT[$n]} listening=$listen  $err"
}

start_one FLServer 5
# Super 起来后 4s 就要连 8001，FL 已在，正常一次过
start_one SuperServer 6
if [ "$(ss -lnt 2>/dev/null | grep -c ':10000 ')" -lt 1 ]; then
  echo "  !! 10000 未监听，重启一次 SuperServer"
  pkill -9 -f "$NB/SuperServer" 2>/dev/null; sleep 2
  start_one SuperServer 6
fi
start_one MiniServer 4
start_one RecordServer 4
start_one BillServer 4
start_one SessionServer 5
start_one GatewayServer 5
start_one ScenesServer 8

# Gateway 的 init 在连上 Super 后要等到 MINI/SESSION 都在 Super 的注册表里才继续 bind 6020。
# Super 的注册表是 tick 里异步刷新的，Gateway 起得太早就会停在那儿不往下走，
# 表现为进程活着、日志停在 setServerInfo、6020 不监听。所以等其它服务都注册完再补一次。
if [ "$(ss -lnt 2>/dev/null | grep -c ':6020 ')" -lt 1 ]; then
  echo "  !! 6020 未监听，等注册表刷新后重启一次 GatewayServer"
  pkill -9 -f "$NB/GatewayServer" 2>/dev/null
  sleep 5
  start_one GatewayServer 6
fi

echo "########## 4. 端口终态 ##########"
for p in 10000 7010 7020 6012 8000 8001 8002 6001 6020 6010; do
  line=$(ss -lntp 2>/dev/null | grep ":$p ")
  if [ -n "$line" ]; then
    who=$(echo "$line" | sed -E 's/.*users:\(\("([^"]+)".*/\1/')
    printf "  %-6s %s\n" "$p" "$who"
  else
    printf "  %-6s --\n" "$p"
  fi
done

echo "########## 5. 判据 ##########"
S=$(ss -lnt 2>/dev/null | grep -c ':6010 ')
if [ "$S" -ge 1 ]; then
  echo "[RESULT] 6010 已监听 —— 整栈原生打通"
else
  echo "[RESULT] 6010 仍未监听，逐服务日志尾巴："
  for n in $ORDER; do
    echo "--- $n ---"
    iconv -f GB18030 -t UTF-8 "$LOGD/$n.out" 2>/dev/null | tail -6
  done
fi
echo "########## BOOT DONE ##########"
