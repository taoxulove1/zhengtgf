#!/bin/bash
TARGET_DIR="/home/Client"
BACKUP_DIR="$TARGET_DIR/copyright_backup_$(date +%Y%m%d_%H%M%S)"
LOG_FILE="$TARGET_DIR/copyright_upgrade_$(date +%Y%m%d_%H%M%S).log"

echo "========================================" | tee "$LOG_FILE"
echo "版权年份升级工具" | tee -a "$LOG_FILE"
echo "========================================" | tee -a "$LOG_FILE"

mkdir -p "$BACKUP_DIR"

echo "正在扫描文件..." | tee -a "$LOG_FILE"
cd "$TARGET_DIR"

find . -type f \( -name "*.cpp" -o -name "*.h" -o -name "*.hpp" -o -name "*.c" -o -name "*.cc" \) 2>/dev/null | grep -v "copyright_backup" | grep -v "backup_" | grep -v "proj_backup" > /tmp/copyright_files.txt

TOTAL=$(cat /tmp/copyright_files.txt 2>/dev/null | wc -l)
echo "找到 $TOTAL 个文件" | tee -a "$LOG_FILE"

if [ "$TOTAL" -eq 0 ]; then
    echo "错误: 没有找到文件!" | tee -a "$LOG_FILE"
    exit 1
fi

FIXED=0
SKIPPED=0

while IFS= read -r file; do
    full_path="$TARGET_DIR/${file#./}"
    rel_path="${full_path#$TARGET_DIR/}"
    
    if [ ! -f "$full_path" ]; then
        continue
    fi
    
    if grep -qi "copyright" "$full_path" 2>/dev/null && \
       (grep -qiE "copyright.*200[0-2]" "$full_path" 2>/dev/null || \
        grep -qiE "copyright.*19[0-9][0-9]" "$full_path" 2>/dev/null || \
        grep -qiE "copyright.*202[0-5]" "$full_path" 2>/dev/null); then
        
        echo "[升级] $rel_path" | tee -a "$LOG_FILE"
        
        backup_path="$BACKUP_DIR/$rel_path"
        mkdir -p "$(dirname "$backup_path")" 2>/dev/null
        cp "$full_path" "$backup_path" 2>/dev/null
        
        sed -i 's/Copyright (c) 2000-2002/Copyright (c) 2000-2026/gi' "$full_path"
        sed -i 's/Copyright (c) 2000-2001/Copyright (c) 2000-2026/gi' "$full_path"
        sed -i 's/Copyright (c) 2001-2002/Copyright (c) 2001-2026/gi' "$full_path"
        sed -i 's/Copyright (c) 2000/Copyright (c) 2000-2026/gi' "$full_path"
        sed -i 's/Copyright (c) 2001/Copyright (c) 2001-2026/gi' "$full_path"
        sed -i 's/Copyright (c) 2002/Copyright (c) 2002-2026/gi' "$full_path"
        sed -i 's/Copyright (c) 19[0-9][0-9]-20[0-1][0-9]/Copyright (c) 2000-2026/gi' "$full_path"
        sed -i 's/Copyright (c) 20[0-1][0-9]-20[0-1][0-9]/Copyright (c) 2000-2026/gi' "$full_path"
        sed -i 's/Copyright (c) 202[0-5]/Copyright (c) 2000-2026/gi' "$full_path"
        sed -i 's/Copyright (c) 19[0-9][0-9]/Copyright (c) 2000-2026/gi' "$full_path"
        
        echo "  OK 更新版权年份" | tee -a "$LOG_FILE"
        FIXED=$((FIXED + 1))
    else
        echo "[跳过] $rel_path" | tee -a "$LOG_FILE"
        SKIPPED=$((SKIPPED + 1))
    fi
done < /tmp/copyright_files.txt

echo "" | tee -a "$LOG_FILE"
echo "========================================" | tee -a "$LOG_FILE"
echo "完成!" | tee -a "$LOG_FILE"
echo "总文件: $TOTAL" | tee -a "$LOG_FILE"
echo "升级: $FIXED" | tee -a "$LOG_FILE"
echo "跳过: $SKIPPED" | tee -a "$LOG_FILE"
echo "备份: $BACKUP_DIR" | tee -a "$LOG_FILE"

rm -f /tmp/copyright_files.txt
