#!/bin/bash
# native_stack.sh —— 8 个服务整栈换成原生构建（同一套 GCC 产物，保证服务间协议一致）
#
# 背景：混跑实测失败 —— 原生 ScenesServer 向 Wine 版 SuperServer 发 t_Startup_Request 后收不到回包
#       （结构布局不一致，见 docs/evidence/20261127-p3-native-flserver-live.md §7.2）。
#
# 两个已知坑（本脚本都处理）：
#   1) kill 掉 Wine 版服务后，wineserver 仍持有监听 fd，端口显示 "not listening" 但 bind 报 EADDRINUSE
#      -> 必须 wineserver -k 彻底释放；副作用是 mysqld 也被杀，随后要重启 MySQL4.0
#   2) config.xml 的 <logfilename>log/xxx</logfilename> 在原版布局里是**目录**，
#      原生（log4cxx）要写文件 -> 报 "IO Exception status code = 21"(EISDIR) 后丢弃日志
#      -> 统一改成 log/nat/xxx.log
set -u
export WINEPREFIX=/root/.wine32
export WINEARCH=win32
export DISPLAY=:99
export WINEDEBUG=-all
export PATH=/opt/wine32/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin

NB=/root/p3-test/p3-934bf0d.vl0cYiyD/zhengtugf-p3-934bf0d/build/p3-native
SRV=/root/zt/server
LOGD=/root/zt/log/native
OUT=/root/zt/run
mkdir -p "$LOGD" "$OUT" "$SRV/log/nat"

declare -A PORT=(
  [SuperServer]=10000 [RecordServer]=7010 [BillServer]=7020 [MiniServer]=6012
  [FLServer]=8000 [SessionServer]=6001 [GatewayServer]=6020 [ScenesServer]=6010
)
ORDER="SuperServer RecordServer BillServer MiniServer FLServer SessionServer GatewayServer ScenesServer"

echo "########## 0. 彻底释放上一次的 socket ##########"
pkill -9 -f 'Server_r\.exe' 2>/dev/null
pkill -9 -f "$NB/" 2>/dev/null
sleep 2
wineserver -k 2>/dev/null
sleep 3
echo "mysqld 存活: $(pgrep -fc 'mysqld\.exe' || echo 0)"

echo "########## 0.1 修 logfilename（目录 -> 文件，幂等）##########"
python3 - <<'EOF'
import re, os
p = "/root/zt/server/config.xml"
d = open(p, "rb").read()
os.makedirs("/root/zt/server/log/nat", exist_ok=True)
def fix(m):
    inner = m.group(1).decode()
    if inner.endswith(".log"):
        return m.group(0)
    base = inner.split("/")[-1]
    return ("<logfilename>log/nat/%s.log</logfilename>" % base).encode()
new, n = re.subn(rb"<logfilename>([^<]+)</logfilename>", fix, d)
if n:
    open(p + ".bak-logfile", "wb").write(d)
    open(p, "wb").write(new)
print("logfilename 已规范化 %d 处" % n)
EOF

echo "########## 0.2 重启 MySQL 4.0 ##########"
bash /root/start_mysql40.sh 2>&1 | grep -E '3307|成功|LISTEN' | head -5

echo "########## 1. 逐个起原生服务 ##########"
for n in $ORDER; do
  bin="$NB/$n"
  if [ ! -x "$bin" ]; then echo "[$n] 缺可执行文件"; continue; fi
  log="$LOGD/$n.out"
  : > "$log"
  ( cd "$SRV" && nohup "$bin" >> "$log" 2>&1 & echo $! > "$OUT/$n.pid" )
  sleep 4
  pid=$(cat "$OUT/$n.pid" 2>/dev/null || echo "")
  alive=no; kill -0 "$pid" 2>/dev/null && alive=yes
  port=${PORT[$n]}
  listen=$(ss -lnt 2>/dev/null | grep -c ":$port ")
  tailmsg=$(iconv -f GB18030 -t UTF-8 "$log" 2>/dev/null | grep -aE 'ERROR|FATAL' | tail -1)
  echo "[$n] pid=$pid alive=$alive port:$port listening=$listen  ${tailmsg}"
done

echo "########## 2. 端口终态 ##########"
for p in 10000 7010 7020 6012 8000 8001 8002 6001 6020 6010; do
  line=$(ss -lntp 2>/dev/null | grep ":$p ")
  if [ -n "$line" ]; then
    who=$(echo "$line" | sed -E 's/.*users:\(\("([^"]+)".*/\1/')
    echo "  $p  $who"
  else
    echo "  $p  --"
  fi
done

echo "########## 3. 存活进程 ##########"
ps aux | grep -E "$NB/" | grep -v grep | awk '{print $2, $11}'
