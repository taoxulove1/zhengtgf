#!/bin/sh
SERVER_BASE="/home/server"
SERVER_DIRS="BillServer FLServer GatewayServer MiniServer RecordServer ScenesServer SessionServer SuperServer"

case "$1" in
    scan)
        echo "=== 扫描服务器目录 ==="
        for dir in $SERVER_DIRS; do
            path="$SERVER_BASE/$dir"
            if [ -d "$path" ]; then
                count=$(find "$path" -type f 2>/dev/null | wc -l)
                size=$(du -sh "$path" 2>/dev/null | cut -f1)
                echo "[$dir] 文件数: $count, 大小: $size"
            fi
        done
        ;;
    detail)
        if [ -z "$2" ]; then
            echo "用法: $0 detail <服务器名>"
            exit 1
        fi
        echo "=== $2 文件列表 ==="
        /bin/ls -la "$SERVER_BASE/$2/" | head -20
        ;;
    version)
        echo "=== 版本检测 ==="
        for dir in $SERVER_DIRS; do
            ver="unknown"
            [ -f "$SERVER_BASE/$dir/version.txt" ] && ver=$(cat "$SERVER_BASE/$dir/version.txt")
            echo "[$dir] 版本: $ver"
        done
        ;;
    backup)
        BACKUP_DIR="$SERVER_BASE/.backup"
        /bin/mkdir -p "$BACKUP_DIR"
        if [ -n "$2" ]; then
            echo "备份 $2..."
            cd "$SERVER_BASE" && /bin/tar czf "$BACKUP_DIR/${2}_$(date +%Y%m%d_%H%M%S).tar.gz" "$2"
        else
            echo "备份所有服务器..."
            for dir in $SERVER_DIRS; do
                cd "$SERVER_BASE" && /bin/tar czf "$BACKUP_DIR/${dir}_$(date +%Y%m%d_%H%M%S).tar.gz" "$dir"
            done
        fi
        echo "备份完成"
        /bin/ls -lh "$BACKUP_DIR"/*.tar.gz 2>/dev/null | tail -5
        ;;
    list)
        echo "=== 备份列表 ==="
        /bin/ls -lh "$SERVER_BASE/.backup/"*.tar.gz 2>/dev/null || echo "无备份文件"
        ;;
    *)
        echo "服务器版本管理工具"
        echo ""
        echo "用法: /bin/sh $0 {命令}"
        echo ""
        echo "命令:"
        echo "  scan              扫描所有服务器目录"
        echo "  detail <服务器名>  查看指定服务器详细文件"
        echo "  version           检测所有服务器版本"
        echo "  backup [服务器名]  备份指定或所有服务器"
        echo "  list              查看备份列表"
        echo ""
        ;;
esac
