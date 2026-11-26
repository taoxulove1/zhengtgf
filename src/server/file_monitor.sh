#!/bin/sh

SERVER_BASE="/home/server"
CHECKSUM_FILE="$SERVER_BASE/.file_checksums"
ALERT_FILE="$SERVER_BASE/.upgrade_alert"

case "$1" in
    scan)
        echo "=== 扫描文件基准 ==="
        find $SERVER_BASE/BillServer $SERVER_BASE/FLServer $SERVER_BASE/GatewayServer \
             $SERVER_BASE/MiniServer $SERVER_BASE/RecordServer $SERVER_BASE/ScenesServer \
             $SERVER_BASE/SessionServer $SERVER_BASE/SuperServer \
             -type f -exec md5sum {} \; 2>/dev/null > "$CHECKSUM_FILE"
        echo "✓ 已记录 $(wc -l < "$CHECKSUM_FILE") 个文件"
        rm -f "$ALERT_FILE"
        ;;
        
    check)
        echo "=== 检测文件变化 ==="
        [ ! -f "$CHECKSUM_FILE" ] && echo "✗ 请先执行 scan" && exit 1
        
        # 检测新文件和修改
        find $SERVER_BASE/BillServer $SERVER_BASE/FLServer $SERVER_BASE/GatewayServer \
             $SERVER_BASE/MiniServer $SERVER_BASE/RecordServer $SERVER_BASE/ScenesServer \
             $SERVER_BASE/SessionServer $SERVER_BASE/SuperServer \
             -type f 2>/dev/null | while read file; do
            rel_path="${file#$SERVER_BASE/}"
            current=$(md5sum "$file" 2>/dev/null | awk '{print $1}')
            recorded=$(grep -F "$rel_path" "$CHECKSUM_FILE" 2>/dev/null | awk '{print $1}')
            
            if [ -z "$recorded" ]; then
                echo "[NEW] $rel_path"
                echo "[NEW] $rel_path" >> "$ALERT_FILE.tmp"
            elif [ "$current" != "$recorded" ]; then
                echo "[MODIFIED] $rel_path"
                echo "[MODIFIED] $rel_path" >> "$ALERT_FILE.tmp"
            fi
        done
        
        # 检测删除的文件
        grep -v '^$' "$CHECKSUM_FILE" | while read line; do
            file=$(echo "$line" | awk '{print $2}')
            [ ! -f "$SERVER_BASE/$file" ] && echo "[DELETED] $file" && echo "[DELETED] $file" >> "$ALERT_FILE.tmp"
        done
        
        if [ -f "$ALERT_FILE.tmp" ]; then
            echo "" 
            echo "⚠ 检测到文件变化！"
            echo "文件变化提醒 - $(date)" > "$ALERT_FILE"
            echo "========================================" >> "$ALERT_FILE"
            cat "$ALERT_FILE.tmp" >> "$ALERT_FILE"
            echo "========================================" >> "$ALERT_FILE"
            echo "" >> "$ALERT_FILE"
            echo "建议:" >> "$ALERT_FILE"
            echo "  1. 备份: /bin/sh server_version_manager.sh backup" >> "$ALERT_FILE"
            echo "  2. 升级: echo '新版本' > version.txt" >> "$ALERT_FILE"
            echo "  3. 重扫: /bin/sh file_monitor.sh scan" >> "$ALERT_FILE"
            rm -f "$ALERT_FILE.tmp"
        else
            echo ""
            echo "✓ 没有文件变化"
            rm -f "$ALERT_FILE"
        fi
        ;;
        
    report)
        [ -f "$ALERT_FILE" ] && cat "$ALERT_FILE" || echo "没有待处理的提醒"
        ;;
        
    clear)
        rm -f "$ALERT_FILE"
        echo "✓ 提醒已清除"
        ;;
        
    *)
        echo "文件监控工具"
        echo "  scan   - 建立基准"
        echo "  check  - 检测变化"
        echo "  report - 查看报告"
        echo "  clear  - 清除提醒"
        ;;
esac
