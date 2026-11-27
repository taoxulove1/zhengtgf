#!/bin/bash
# zt_monitor.sh —— 征途全栈常驻监控（每 15s 一轮）
# 职责：8 服务进程存活+自动拉起 / 端口监听 / 玩家连接计数 / 服务日志 ERROR 增量扫描
#      MySQL(Wine 3307) / HTTP:80 / 磁盘内存 / http.log 请求与 4xx/5xx 告警
# 输出：/root/zt/monitor/monitor.log（异常行带 [ALERT]，玩家上线/心跳为 note 行）

NB=/root/p3-test/p3-934bf0d.vl0cYiyD/zhengtugf-p3-934bf0d/build/p3-native
SRV=/root/zt/server
LOGD=/root/zt/log/native
OUT=/root/zt/run
MON=/root/zt/monitor
WWWLOG=/root/zt/www/http.log
ORDER="FLServer SuperServer MiniServer RecordServer BillServer SessionServer GatewayServer ScenesServer"
declare -A PORT=( [SuperServer]=10000 [RecordServer]=7010 [BillServer]=7020 [MiniServer]=6012
                  [FLServer]=8000 [SessionServer]=6001 [GatewayServer]=6020 [ScenesServer]=6010 )
declare -A CUR   # 日志增量扫描游标（字节偏移）
mkdir -p "$MON"

ts() { date '+%m-%d %H:%M:%S'; }
alert() { echo "[$(ts)] [ALERT] $*" >> "$MON/monitor.log"; }
note()  { echo "[$(ts)] $*" >> "$MON/monitor.log"; }

note "=== zt_monitor started pid=$$ ==="
REQ_TOTAL=0
SEQ=0
while true; do
  down=0

  # 1) 服务进程存活 + 端口监听 + 自动拉起
  #    判活用 pgrep -x 进程名（pid 文件可能被段错误实例污染），
  #    拉起前先查端口占用防 bind 冲突段错误风暴，每轮最多拉起 2 个（熔断）。
  for n in $ORDER; do
    if ! pgrep -x "$n" >/dev/null 2>&1; then
      if ! ss -lnt 2>/dev/null | grep -q ":${PORT[$n]} "; then
        if [ "$down" -ge 2 ]; then
          alert "拉起风暴熔断：$n 需拉起但本轮已拉起 $down 个，跳过"
          continue
        fi
        alert "$n 进程死亡且端口 ${PORT[$n]} 未监听 —— 自动拉起"
        : > "$LOGD/$n.out"
        ( cd "$SRV" && nohup "$NB/$n" >> "$LOGD/$n.out" 2>&1 & echo $! > "$OUT/$n.pid" ) 2>/dev/null
        sleep 3
        if pgrep -x "$n" >/dev/null 2>&1; then
          note "$n 拉起成功"
        else
          alert "$n 拉起后仍未存活（疑似启动崩溃）"
        fi
        down=$((down+1))
      else
        alert "$n pgrep 未命中但端口 ${PORT[$n]} 在监听 —— 跳过拉起（实例可能活着）"
      fi
    fi
    if ! ss -lnt 2>/dev/null | grep -q ":${PORT[$n]} "; then
      alert "$n 端口 ${PORT[$n]} 未监听"
    fi
  done

  # 2) MySQL(Wine mysqld 3307)
  if ! pgrep -f mysqld >/dev/null; then
    alert "MySQL(Wine 3307) 死亡 后台拉起"
    ( bash /root/start_mysql40.sh > "$LOGD/mysql40.out" 2>&1 & )
  fi

  # 3) HTTP:80（官方登录器更新源）
  if ! ss -lnt 2>/dev/null | grep -q ':80 '; then
    alert "HTTP:80 未监听 拉起 threaded_server"
    ( cd /root/zt/www && nohup python3 /root/zt/www/threaded_server.py 80 /root/zt/www </dev/null >> "$WWWLOG" 2>&1 & )
  fi

  # 4) 玩家连接计数（ss -nt 默认不含 LISTEN；FLServer 8000 / GatewayServer 6020）
  e8000=$(ss -nt 2>/dev/null | awk '$4 ~ /:8000$/' | wc -l)
  e6020=$(ss -nt 2>/dev/null | awk '$4 ~ /:6020$/' | wc -l)
  if [ "$e8000" -gt 0 ] || [ "$e6020" -gt 0 ]; then
    note "玩家连接 FL8000=$e8000 GW6020=$e6020"
  fi

  # 5) 服务日志 ERROR/FATAL 增量扫描（原始字节 grep，GB18030/UTF-8 双兼容）
  for n in $ORDER; do
    log="$LOGD/$n.out"
    [ -f "$log" ] || continue
    sz=$(stat -c %s "$log" 2>/dev/null || echo 0)
    cur=${CUR[$n]:-0}
    if [ "$sz" -gt "$cur" ]; then
      tail -c +$((cur+1)) "$log" | grep -aE 'ERROR|FATAL|ASSERT|Segmentation' | cut -c1-200 | \
        while IFS= read -r line; do [ -n "$line" ] && alert "$n 日志: $line"; done
      CUR[$n]=$sz
    elif [ "$sz" -lt "$cur" ]; then
      CUR[$n]=$sz   # 日志被清空（拉起时 : > log）
    fi
  done

  # 6) http.log 增量：请求计数 + 4xx/5xx 告警
  if [ -f "$WWWLOG" ]; then
    sz=$(stat -c %s "$WWWLOG")
    cur=${CUR[www]:-0}
    if [ "$sz" -gt "$cur" ]; then
      seg=$(tail -c +$((cur+1)) "$WWWLOG")
      req=$(printf '%s\n' "$seg" | grep -ac 'GET\|POST\|HEAD' || true)
      bad=$(printf '%s\n' "$seg" | grep -acE '" (40[0-9]|50[0-9]) ' || true)
      [ "$bad" -gt 0 ] && alert "http.log 增量 ${req} 请求中 ${bad} 个 4xx/5xx"
      CUR[www]=$sz
      REQ_TOTAL=$((REQ_TOTAL + req))
    elif [ "$sz" -lt "$cur" ]; then
      CUR[www]=$sz
    fi
  fi

  # 7) 资源水位
  disk=$(df / | awk 'NR==2{gsub("%","",$5);print $5}')
  [ "$disk" -ge 90 ] && alert "磁盘 / 使用率 ${disk}%"
  mem=$(free | awk '/Mem:/{print int($3/$2*100)}')
  [ "$mem" -ge 90 ] && alert "内存使用率 ${mem}%"

  # 8) 心跳（每 4 轮 = 1 分钟）
  if [ $((SEQ % 4)) -eq 0 ]; then
    up=0
    for n in $ORDER; do
      pgrep -x "$n" >/dev/null 2>&1 && up=$((up+1))
    done
    note "心跳 服务存活 $up/8 conn8000=$e8000 conn6020=$e6020 磁盘=${disk}% 内存=${mem}% 本轮拉起=$down 累计请求=$REQ_TOTAL"
  fi
  SEQ=$((SEQ+1))
  sleep 15
done
