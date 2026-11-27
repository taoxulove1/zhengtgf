#!/bin/bash
# native_trial.sh —— 一次性试验：整栈原生 + 已修好的 Super + 补过 schema 的 DB
# 判据：6010(Scenes) 起来 => 成功，保留；否则自动回滚到已验证可用的混合栈。
#
# 试验前已完成的两处修复：
#   1) SuperServer/ServerTask.cpp：去掉 initServerSequence 的 constructor 属性，改显式调用（已重编）
#   2) Zebra.SEPT 补 BOSSEXP 列 + 造 /root/zt/server/Config/GameConfig.xml（<Config></Config>）
set -u
export WINEPREFIX=/root/.wine32
export WINEARCH=win32
export DISPLAY=:99
export WINEDEBUG=-all
export PATH=/opt/wine32/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin

CFG=/root/zt/server/config.xml
NB=/root/p3-test/p3-934bf0d.vl0cYiyD/zhengtugf-p3-934bf0d/build/p3-native
LOGD=/root/zt/log/native
mkdir -p "$LOGD"

patch_native() {
  sed -i 's#<ifname>10.1.0.10</ifname>#<ifname>eth0</ifname>#' "$CFG"
  python3 - <<'EOF'
import re
p="/root/zt/server/config.xml"
d=open(p,"rb").read()
d2,n=re.subn(rb"<logfilename>log/([A-Za-z]+)</logfilename>", rb"<logfilename>log/nat/\1.log</logfilename>", d)
if n: open(p,"wb").write(d2)
print("logfilename -> log/nat/*.log :", n)
EOF
}

patch_wine() {
  sed -i 's#<ifname>eth0</ifname>#<ifname>10.1.0.10</ifname>#' "$CFG"
  python3 - <<'EOF'
import re
p="/root/zt/server/config.xml"
d=open(p,"rb").read()
d2,n=re.subn(rb"<logfilename>log/nat/([A-Za-z]+)\.log</logfilename>", rb"<logfilename>log/\1</logfilename>", d)
if n: open(p,"wb").write(d2)
print("logfilename 回滚:", n)
EOF
}

rollback_mixed() {
  echo "########## 回滚到混合栈（原生 FL + Wine 其余）##########"
  pkill -9 -f '/build/p3-native/' 2>/dev/null
  sleep 2
  patch_wine
  bash /root/native_cutover.sh > /root/zt/log/rollback.log 2>&1
  tail -14 /root/zt/log/rollback.log
}

echo "########## 1. 切到原生配置 ##########"
patch_native

echo "########## 2. 起整栈原生 ##########"
pkill -9 -f 'Server_r\.exe' 2>/dev/null
pkill -9 -f '/build/p3-native/' 2>/dev/null
sleep 2
wineserver -k 2>/dev/null
sleep 3
mkdir -p /root/zt/server/log/nat
bash /root/start_mysql40.sh > /root/zt/log/native/mysql40.out 2>&1
echo "mysqld: $(pgrep -fc 'mysqld\.exe' || echo 0)  3307: $(ss -lnt 2>/dev/null | grep -c ':3307 ')"

ORDER="SuperServer RecordServer BillServer MiniServer FLServer SessionServer GatewayServer ScenesServer"
declare -A PORT=( [SuperServer]=10000 [RecordServer]=7010 [BillServer]=7020 [MiniServer]=6012
                  [FLServer]=8000 [SessionServer]=6001 [GatewayServer]=6020 [ScenesServer]=6010 )
for n in $ORDER; do
  : > "$LOGD/$n.out"
  ( cd /root/zt/server && nohup "$NB/$n" >> "$LOGD/$n.out" 2>&1 & )
  sleep 6
  listen=$(ss -lnt 2>/dev/null | grep -c ":${PORT[$n]} ")
  err=$(iconv -f GB18030 -t UTF-8 "$LOGD/$n.out" 2>/dev/null | grep -aE 'ERROR|FATAL' | tail -1 | cut -c1-120)
  echo "[$n] port=${PORT[$n]} listening=$listen  $err"
done

echo "########## 3. 判据 ##########"
SCENE=$(ss -lnt 2>/dev/null | grep -c ':6010 ')
echo "6010 listening=$SCENE"
for p in 10000 7010 7020 6012 8000 8001 8002 6001 6020 6010; do
  printf "  %s %s\n" "$p" "$(ss -lnt 2>/dev/null | grep -c ":$p ")"
done

if [ "$SCENE" -ge 1 ]; then
  echo "[RESULT] 6010 起来了，保留整栈原生"
else
  echo "[RESULT] 6010 仍未起来，自动回滚"
  rollback_mixed
  echo "########## 回滚后端口 ##########"
  for p in 10000 7010 7020 6012 8000 8001 8002 6001 6020 6010; do
    printf "  %s %s\n" "$p" "$(ss -lnt 2>/dev/null | grep -c ":$p ")"
  done
fi
echo "########## TRIAL DONE ##########"
