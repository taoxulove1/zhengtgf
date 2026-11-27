#!/bin/bash
# fix_tbl_missing.sh —— 缺表降级：数据表不存在时告警并跳过，不再让 loadAllBM() 直接失败退出
#
# 实测缺口（tools/native_boot.sh + scenes_gdb.sh 之后）：
#   服务器 /root/zt/server/map/ 只有 25 张 .tbl，
#   而 src/server/base/zDatabaseManager.cpp 的 loadAllBM() 要 70 张。
#   多出来的是这套源码后期加的系统（法宝/元神/龙魂/勋章…），这套老数据本来就没有。
#   loadAllBM() 遇第一张缺表就 return false -> ScenesService::init 失败 -> 进程退出 -> 6010 消失。
#
# 处置分两类：
#   1) 真有数据的：ExperienceBase.tbl 已从本地 resource-intake/map-candidates 补进 map/（真实经验表）。
#   2) 这套数据本来就没有的扩展表：refresh() 打开失败时把 ret 置 true，
#      日志打 TABLE-MISSING，管理器保持为空（getEntryByID 返回 NULL，老客户端不会触发这些系统）。
#
# 只改 zDatabaseManager.h 的 refresh(const char*) 打开失败分支一行，重编 base + ScenesServer。
set -u
ROOT=/root/p3-test/p3-934bf0d.vl0cYiyD/zhengtugf-p3-934bf0d/src/server
NB=/root/p3-test/p3-934bf0d.vl0cYiyD/zhengtugf-p3-934bf0d/build/p3-native
FLAGS="-m32 -O2 -D_USE_EPOLL_ -D_USE_CMD_NAMESPACE -D_ENCDEC_MSG"
export LIBRARY_PATH=/root/p3-test/p3-lua-deps/lib:/usr/local/lib
export ZEBRA_BOOST_INCLUDE=/root/p3-test/p3-scenes-boost-fix/include
export PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin

echo "########## 1. 补真实经验表 ##########"
ls -l /root/zt/server/map/ExperienceBase.tbl

echo "########## 2. 打补丁 zDatabaseManager.h ##########"
H="$ROOT/base/zDatabaseManager.h"
[ -f "$H.bak-tblmissing" ] || cp "$H" "$H.bak-tblmissing"
python3 - <<'PY'
import re
p = "/root/p3-test/p3-934bf0d.vl0cYiyD/zhengtugf-p3-934bf0d/src/server/base/zDatabaseManager.h"
d = open(p, "rb").read()
if b"TABLE-MISSING" in d:
    print("  已打过补丁，跳过")
    raise SystemExit(0)
# 打开文件失败的那个 error(... %s",filename) 只此一处
pat = re.compile(rb'Zebra::logger->error\("[^"\r\n]* %s",filename\);')
m = pat.search(d)
if not m:
    raise SystemExit("  [ERR] 没匹配到打开失败分支")
old = m.group(0)
new = b'Zebra::logger->error("TABLE-MISSING skip %s",filename); ret=true; /* P3-FIX: missing tbl tolerated */'
d2 = d[:m.start()] + new + d[m.end():]
open(p, "wb").write(d2)
print("  原:", old.decode("gb18030", "replace"))
print("  新:", new.decode())
PY

echo "########## 3. 重编 base + ScenesServer ##########"
for m in base ScenesServer; do
  echo "--- make $m ---"
  make -C "$ROOT/$m" -j4 CC=gcc CXX=g++ "ZEBRA_COMPILE_OPT=$FLAGS" > "/root/tblfix_$m.log" 2>&1
  rc=$?
  echo "  rc=$rc"
  if [ $rc -ne 0 ]; then grep -n -i 'error\|\*\*\*' "/root/tblfix_$m.log" | head -15; exit 1; fi
done
ls -l "$ROOT/ScenesServer/ScenesServer"

echo "########## 4. 部署 ##########"
cp "$ROOT/ScenesServer/ScenesServer" "$NB/ScenesServer"
chmod +x "$NB/ScenesServer"
ls -l "$NB/ScenesServer"
echo "########## TBLFIX DONE ##########"
