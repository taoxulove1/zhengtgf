#!/bin/bash

# l.sh - 完整自动升级脚本
# 处理所有检测到的 VS2003 兼容性问题

TARGET_DIR="/home/Client"
BACKUP_DIR="$TARGET_DIR/backup_$(date +%Y%m%d_%H%M%S)"
LOG_FILE="$TARGET_DIR/upgrade_$(date +%Y%m%d_%H%M%S).log"

# 颜色输出
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m'

# 计数器
TOTAL_FILES=0
UPGRADED_HEADER=0
UPGRADED_UNSAFE=0
UPGRADED_OLD_HEADER=0
SKIPPED=0
ERRORS=0

# 日志函数
log() {
    echo -e "$1" | tee -a "$LOG_FILE"
}

# 创建备份目录
mkdir -p "$BACKUP_DIR"
log "${BLUE}========================================${NC}"
log "${BLUE}VS2003 完整自动升级脚本${NC}"
log "${BLUE}开始时间: $(date)${NC}"
log "${BLUE}备份目录: $BACKUP_DIR${NC}"
log "${BLUE}========================================${NC}"
log ""

# 备份函数
backup_file() {
    local src="$1"
    local rel_path="${src#$TARGET_DIR/}"
    local backup_path="$BACKUP_DIR/$rel_path"
    mkdir -p "$(dirname "$backup_path")" 2>/dev/null
    if cp "$src" "$backup_path" 2>/dev/null; then
        log "${CYAN}[备份]${NC} $rel_path"
        return 0
    else
        log "${RED}[备份失败]${NC} $src"
        return 1
    fi
}

# 添加头文件保护 (pragma once)
fix_header_guard() {
    local file="$1"

    # 检查是否已有保护
    if grep -q "#pragma once" "$file" 2>/dev/null || grep -q "#ifndef" "$file" 2>/dev/null; then
        return 1
    fi

    # 创建临时文件
    local temp_file=$(mktemp)

    # 添加 pragma once
    echo "#pragma once  // [UPGRADED] Auto-added header guard" > "$temp_file"
    echo "" >> "$temp_file"
    cat "$file" >> "$temp_file"

    mv "$temp_file" "$file"
    return 0
}

# 修复不安全函数
fix_unsafe_functions() {
    local file="$1"
    local temp_file=$(mktemp)
    local modified=0

    while IFS= read -r line || [[ -n "$line" ]]; do
        local original="$line"
        local new_line="$line"
        local comment=""

        # strcpy -> strcpy_s (注意：这只是一个标记，实际需要手动添加大小参数)
        if echo "$line" | grep -qE '\bstrcpy[[:space:]]*\('; then
            new_line=$(echo "$line" | sed 's/\bstrcpy[[:space:]]*\(/strcpy_s(/g')
            comment="strcpy->strcpy_s"
            ((modified++))
        fi

        # strcat -> strcat_s
        if echo "$line" | grep -qE '\bstrcat[[:space:]]*\('; then
            new_line=$(echo "$line" | sed 's/\bstrcat[[:space:]]*\(/strcat_s(/g')
            comment="strcat->strcat_s"
            ((modified++))
        fi

        # sprintf -> sprintf_s (排除 snprintf)
        if echo "$line" | grep -qE '\bsprintf[[:space:]]*\(' && ! echo "$line" | grep -qE 'snprintf'; then
            new_line=$(echo "$line" | sed 's/\bsprintf[[:space:]]*\(/sprintf_s(/g')
            comment="sprintf->sprintf_s"
            ((modified++))
        fi

        # gets -> fgets
        if echo "$line" | grep -qE '\bgets[[:space:]]*\('; then
            new_line=$(echo "$line" | sed 's/\bgets[[:space:]]*\(/fgets(/g')
            comment="gets->fgets"
            ((modified++))
        fi

        # wcscpy -> wcscpy_s
        if echo "$line" | grep -qE '\bwcscpy[[:space:]]*\('; then
            new_line=$(echo "$line" | sed 's/\bwcscpy[[:space:]]*\(/wcscpy_s(/g')
            comment="wcscpy->wcscpy_s"
            ((modified++))
        fi

        # wcscat -> wcscat_s
        if echo "$line" | grep -qE '\bwcscat[[:space:]]*\('; then
            new_line=$(echo "$line" | sed 's/\bwcscat[[:space:]]*\(/wcscat_s(/g')
            comment="wcscat->wcscat_s"
            ((modified++))
        fi

        # 输出注释和修改后的行
        if [[ -n "$comment" ]]; then
            echo "// [UPGRADED] $comment"
        fi
        echo "$new_line"

    done < "$file" > "$temp_file"

    if [ $modified -gt 0 ]; then
        mv "$temp_file" "$file"
        return 0
    else
        rm "$temp_file"
        return 1
    fi
}

# 修复旧式头文件
fix_old_headers() {
    local file="$1"
    local temp_file=$(mktemp)
    local modified=0
    local added_namespace=0

    while IFS= read -r line || [[ -n "$line" ]]; do
        local new_line="$line"

        # iostream.h -> iostream
        if echo "$line" | grep -qiE '#include[[:space:]]*<iostream\.h>'; then
            new_line="#include <iostream>  // [UPGRADED] iostream.h -> iostream"
            ((modified++))
            added_namespace=1
        fi

        # fstream.h -> fstream
        if echo "$line" | grep -qiE '#include[[:space:]]*<fstream\.h>'; then
            new_line="#include <fstream>  // [UPGRADED] fstream.h -> fstream"
            ((modified++))
        fi

        # string.h -> cstring (C++ 风格，但保留 C 风格如果看起来是 C 文件)
        if echo "$line" | grep -qiE '#include[[:space:]]*<string\.h>'; then
            # 检查是否是 C++ 文件
            if [[ "$file" == *.cpp ]] || [[ "$file" == *.hpp ]] || [[ "$file" == *.cc ]]; then
                new_line="#include <cstring>  // [UPGRADED] string.h -> cstring (C++ style)"
            else
                new_line="#include <string.h>  // [NOTE] Kept C style for C file"
            fi
            ((modified++))
        fi

        # vector.h -> vector
        if echo "$line" | grep -qiE '#include[[:space:]]*<vector\.h>'; then
            new_line="#include <vector>  // [UPGRADED] vector.h -> vector"
            ((modified++))
        fi

        # map.h -> map
        if echo "$line" | grep -qiE '#include[[:space:]]*<map\.h>'; then
            new_line="#include <map>  // [UPGRADED] map.h -> map"
            ((modified++))
        fi

        # set.h -> set
        if echo "$line" | grep -qiE '#include[[:space:]]*<set\.h>'; then
            new_line="#include <set>  // [UPGRADED] set.h -> set"
            ((modified++))
        fi

        # list.h -> list
        if echo "$line" | grep -qiE '#include[[:space:]]*<list\.h>'; then
            new_line="#include <list>  // [UPGRADED] list.h -> list"
            ((modified++))
        fi

        echo "$new_line"

    done < "$file" > "$temp_file"

    # 如果需要，在第一个 #include 后添加 using namespace std
    if [ $added_namespace -eq 1 ]; then
        local temp_file2=$(mktemp)
        local first_include=0
        while IFS= read -r line || [[ -n "$line" ]]; do
            echo "$line"
            if [ $first_include -eq 0 ] && echo "$line" | grep -qE '#include'; then
                echo ""
                echo "using namespace std;  // [UPGRADED] Added for modern C++ headers"
                first_include=1
            fi
        done < "$temp_file" > "$temp_file2"
        mv "$temp_file2" "$temp_file"
    fi

    if [ $modified -gt 0 ]; then
        mv "$temp_file" "$file"
        return 0
    else
        rm "$temp_file"
        return 1
    fi
}

# 处理单个文件
process_file() {
    local file="$1"
    local issues="$2"  # 空格分隔的问题列表

    ((TOTAL_FILES++))

    if [ ! -f "$file" ]; then
        log "${RED}[跳过]${NC} 文件不存在: $file"
        ((SKIPPED++))
        return
    fi

    # 跳过 boost 库文件（通常是第三方库，不建议修改）
    if echo "$file" | grep -qE 'boost_[0-9]+_[0-9]+_[0-9]+|/boost/'; then
        log "${YELLOW}[跳过第三方库]${NC} $file"
        ((SKIPPED++))
        return
    fi

    # 跳过 lua 库文件
    if echo "$file" | grep -qE 'lua-[0-9]\.[0-9]'; then
        log "${YELLOW}[跳过第三方库]${NC} $file"
        ((SKIPPED++))
        return
    fi

    local rel_path="${file#$TARGET_DIR/}"
    log "${BLUE}[处理]${NC} $rel_path"
    log "       问题: $issues"

    # 备份
    backup_file "$file"

    local fixed=0

    # 修复头文件保护
    if echo "$issues" | grep -q "缺少头文件保护"; then
        if [[ "$file" == *.h ]] || [[ "$file" == *.hpp ]]; then
            if fix_header_guard "$file"; then
                log "       ${GREEN}✓ 添加头文件保护${NC}"
                ((UPGRADED_HEADER++))
                fixed=1
            else
                log "       ${YELLOW}⚠ 已有保护或跳过${NC}"
            fi
        fi
    fi

    # 修复不安全函数
    if echo "$issues" | grep -q "不安全函数"; then
        if fix_unsafe_functions "$file"; then
            log "       ${GREEN}✓ 修复不安全函数${NC}"
            ((UPGRADED_UNSAFE++))
            fixed=1
        else
            log "       ${YELLOW}⚠ 无不安全函数或跳过${NC}"
        fi
    fi

    # 修复旧式头文件
    if echo "$issues" | grep -q "旧式头文件"; then
        if fix_old_headers "$file"; then
            log "       ${GREEN}✓ 升级头文件${NC}"
            ((UPGRADED_OLD_HEADER++))
            fixed=1
        else
            log "       ${YELLOW}⚠ 无旧式头文件或跳过${NC}"
        fi
    fi

    if [ $fixed -eq 0 ]; then
        ((SKIPPED++))
    fi

    log ""
}

# ==================== 主程序 ====================

log "${CYAN}开始处理所有检测到的文件...${NC}"
log ""

# 定义所有需要处理的文件和它们的问题
# 格式: "文件路径|问题1 问题2 问题3"

declare -a FILE_LIST=(
    # Magic 目录
    "/home/Client/Magic/public.h|不安全函数"
    "/home/Client/Magic/cGround.cpp|不安全函数"
    "/home/Client/Magic/cMiniGround.cpp|不安全函数"
    "/home/Client/Magic/CharacterOrder.h|缺少头文件保护"
    "/home/Client/Magic/MapSoundManager.cpp|不安全函数"
    "/home/Client/Magic/cMagic.cpp|不安全函数"
    "/home/Client/Magic/stringex.cpp|旧式头文件 不安全函数"
    "/home/Client/Magic/cAnimation.cpp|不安全函数"
    "/home/Client/Magic/stringex.h|不安全函数"
    "/home/Client/Magic/iCharacter.h|不安全函数"
    "/home/Client/Magic/magic_sound.h|缺少头文件保护"
    "/home/Client/Magic/cCharacter.cpp|不安全函数"
    "/home/Client/Magic/MapScene.cpp|不安全函数"
    "/home/Client/Magic/cAnimationArray.h|缺少头文件保护"
    "/home/Client/Magic/magic_sound.cpp|不安全函数"

    # RenderD3D 目录
    "/home/Client/RenderD3D/source/D3DFont.cpp|不安全函数"
    "/home/Client/RenderD3D/source/D3DDevice.cpp|不安全函数"
    "/home/Client/RenderD3D/source/D3DBitmap.cpp|不安全函数"

    # Property 目录
    "/home/Client/Property/cPropertyTable.cpp|不安全函数"
    "/home/Client/Property/iProperty.h|缺少头文件保护 不安全函数"
    "/home/Client/Property/cFileOP.cpp|不安全函数"
    "/home/Client/Property/cFileOP.h|不安全函数"

    # MakeExe
    "/home/Client/MakeExe/MakeExeDlg.cpp|不安全函数"

    # Engine 目录
    "/home/Client/engine/source/winFileio.cpp|不安全函数"
    "/home/Client/engine/source/Appation.cpp|不安全函数"
    "/home/Client/engine/source/engine.cpp|不安全函数"
    "/home/Client/engine/source/stringex.cpp|旧式头文件 不安全函数"
    "/home/Client/engine/source/Ini.cpp|不安全函数"
    "/home/Client/engine/source/PackAggregate.cpp|不安全函数"
    "/home/Client/engine/source/FilePackHelper.cpp|不安全函数"
    "/home/Client/engine/source/ZipAggregate.cpp|不安全函数"
    "/home/Client/engine/include/TBitmapList.h|不安全函数"
    "/home/Client/engine/include/TBitmaps.h|缺少头文件保护"
    "/home/Client/engine/include/stringex.h|不安全函数"
    "/home/Client/engine/include/Log.h|不安全函数"
    "/home/Client/engine/include/IDevice.h|不安全函数"
)

# 处理每个文件
for entry in "${FILE_LIST[@]}"; do
    IFS='|' read -r file_path issues <<< "$entry"
    process_file "$file_path" "$issues"
done

# 统计输出
log "${BLUE}========================================${NC}"
log "${BLUE}升级完成统计${NC}"
log "${BLUE}========================================${NC}"
log "总处理文件: $TOTAL_FILES"
log "添加头文件保护: $UPGRADED_HEADER"
log "修复不安全函数: $UPGRADED_UNSAFE"
log "升级旧式头文件: $UPGRADED_OLD_HEADER"
log "跳过/失败: $SKIPPED"
log "错误: $ERRORS"
log ""
log "备份位置: ${CYAN}$BACKUP_DIR${NC}"
log "日志文件: ${CYAN}$LOG_FILE${NC}"
log ""
log "${YELLOW}⚠️  重要提醒:${NC}"
log "1. strcpy_s/sprintf_s 需要额外的缓冲区大小参数，请手动检查并添加"
log "2. 例如: strcpy_s(dest, sizeof(dest), src)"
log "3. 第三方库文件(boost/lua)已自动跳过，建议单独升级或替换"
log "4. 编译时可能需要添加预处理器定义: _CRT_SECURE_NO_WARNINGS"
log ""
log "${GREEN}完成!${NC}"

# 生成检查清单
cat > "$TARGET_DIR/manual_fix_checklist.txt" << 'CHECKLIST'
========================================
手动修复检查清单 (l.sh 生成)
========================================

以下函数替换后需要手动添加缓冲区大小参数：

1. strcpy_s 用法:
   原: strcpy(dest, src);
   新: strcpy_s(dest, sizeof(dest), src);

2. strcat_s 用法:
   原: strcat(dest, src);
   新: strcat_s(dest, sizeof(dest), src);

3. sprintf_s 用法:
   原: sprintf(buffer, "%s", str);
   新: sprintf_s(buffer, sizeof(buffer), "%s", str);

搜索以下标记进行修复:
  grep -r "strcpy_s(" /home/Client --include="*.cpp" --include="*.h"
  grep -r "sprintf_s(" /home/Client --include="*.cpp" --include="*.h"

第三方库处理建议:
  - boost_1_34_1: 建议升级到最新版 boost 或定义 _CRT_SECURE_NO_WARNINGS
  - lua-5.0.2: 建议升级到 Lua 5.4 或在项目设置中忽略这些警告

编译设置:
  项目属性 -> C/C++ -> 预处理器 -> 预处理器定义 -> 添加:
    _CRT_SECURE_NO_WARNINGS
    _CRT_NONSTDC_NO_WARNINGS
    _SCL_SECURE_NO_WARNINGS

========================================
CHECKLIST

log "${CYAN}已生成手动修复检查清单: manual_fix_checklist.txt${NC}"
