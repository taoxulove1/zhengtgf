#!/bin/bash
# link_scenes.sh —— 补齐 base/EncDec + base 静态库，然后链接 ScenesServer
#
# 前置状态（tools/fix_scenes_uninit.sh 实测）：
#   ScenesServer.h 的构造函数补丁已生效，ScenesServer/ 下 89 个 .o 全部编译通过。
#   唯一缺的是链接输入：../base/libzebra.a。这棵树当初只做了隔离构建，
#   临时 work 目录随构建结束被删，所以本树 base 从未产出 .a。
#   本次把 base/EncDec、base 两个模块在本树里建出来，再链接 ScenesServer。
set -u
ROOT=/root/p3-test/p3-934bf0d.vl0cYiyD/zhengtugf-p3-934bf0d/src/server
NB=/root/p3-test/p3-934bf0d.vl0cYiyD/zhengtugf-p3-934bf0d/build/p3-native
FLAGS="-m32 -O2 -D_USE_EPOLL_ -D_USE_CMD_NAMESPACE -D_ENCDEC_MSG"
export LIBRARY_PATH=/root/p3-test/p3-lua-deps/lib:/usr/local/lib
export ZEBRA_BOOST_INCLUDE=/root/p3-test/p3-scenes-boost-fix/include
export PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin

for m in base/EncDec base; do
  echo "########## make $m ##########"
  make -C "$ROOT/$m" -j4 CC=gcc CXX=g++ "ZEBRA_COMPILE_OPT=$FLAGS" > "/root/link_${m//\//_}.log" 2>&1
  rc=$?
  echo "  rc=$rc"
  if [ $rc -ne 0 ]; then grep -n -i 'error\|\*\*\*' "/root/link_${m//\//_}.log" | head -15; exit 1; fi
done
ls -l "$ROOT/base/libzebra.a" "$ROOT/base/EncDec/libencdec.a" 2>/dev/null

echo "########## link ScenesServer ##########"
make -C "$ROOT/ScenesServer" -j4 CC=gcc CXX=g++ "ZEBRA_COMPILE_OPT=$FLAGS" > /root/link_ScenesServer.log 2>&1
rc=$?
echo "  rc=$rc"
if [ $rc -ne 0 ]; then grep -n -i 'error\|\*\*\*\|undefined' /root/link_ScenesServer.log | head -25; exit 1; fi
tail -3 /root/link_ScenesServer.log
ls -l "$ROOT/ScenesServer/ScenesServer"

echo "########## 部署到 build/p3-native ##########"
[ -f "$NB/ScenesServer.bak-uninit" ] || cp "$NB/ScenesServer" "$NB/ScenesServer.bak-uninit"
cp "$ROOT/ScenesServer/ScenesServer" "$NB/ScenesServer"
chmod +x "$NB/ScenesServer"
ls -l "$NB/ScenesServer"
echo "########## LINK DONE ##########"
