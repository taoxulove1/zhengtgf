#!/bin/bash
# capture-server.sh —— 在目标 Linux 服务器侧抓包（tcpdump 已装，不需要 Windows 管理员权限）
# 用法：bash capture-server.sh <秒数> <输出文件> [bpf]
#       bash capture-server.sh 30 /root/zt/cap_login.pcapng "tcp and port 8000"
# 依赖：/usr/sbin/tcpdump（CentOS 7 自带 4.9.2）
set -u
SECS=${1:-20}
OUT=${2:-/root/zt/cap_$(date +%H%M%S).pcapng}
BPF=${3:-tcp}

if ! command -v /usr/sbin/tcpdump >/dev/null 2>&1; then
    echo "[ERR] 未找到 /usr/sbin/tcpdump，先 yum install -y tcpdump" >&2
    exit 1
fi

echo "== 抓包 ${SECS}s  接口 any  BPF='$BPF'  输出 $OUT"
# -i any 在 CentOS 7 上是 SLL2/SLL 链路层，tools/pcap_lite.py 两种都解
nohup timeout "$SECS" /usr/sbin/tcpdump -i any -n -s0 -w "$OUT" "$BPF" >/tmp/ztcap.log 2>&1 &
PID=$!
wait $PID
RC=$?

if [ ! -s "$OUT" ]; then
    echo "[ERR] 没产出文件，检查 BPF 或权限" >&2
    cat /tmp/ztcap.log >&2
    exit 2
fi

echo "== 完成 $OUT ($(stat -c%s "$OUT") B)"
echo "== 包数：$(/usr/sbin/tcpdump -r "$OUT" -nn 2>/dev/null | wc -l)"
echo
echo "== 前 15 条"
/usr/sbin/tcpdump -r "$OUT" -nn 2>/dev/null | head -15
echo
echo "取回本地：python tools/remote.py get $OUT captures/$(basename "$OUT")"
echo "解帧    ：python tools/pcap_lite.py captures/$(basename "$OUT") --list"
exit $RC
