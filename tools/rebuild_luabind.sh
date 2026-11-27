#!/bin/bash
# rebuild_luabind.sh —— 用仓库自带 luabind 源码 + Boost 1.34.1 重编 libluabind.a，再全量清编 ScenesServer
#
# 根因（gdb scenes-gdb3 实锤）：
#   Binder::bind(LuaVM*) 注册 luabind 模块时跳野地址 0x08693538（boost::function1::stored_vtable）。
#   libluabind.a 是按 /usr/local/include/boost-1_33_1（已不存在）编的，
#   而 ScenesServer 的 .o 是按 /root/p3-test/p3-scenes-boost-fix/include（Boost 1.34.1）编的。
#   boost::function 的内部布局在两版间不同，luabind 的 overload_rep 跨 ABI 一传就坏。
#
#   luabind 源码在仓库 src/client/luabind/src/*.cpp（已上传 /root/luabind-src.tgz）。
#   头文件直接用 src/server/script/luabind（含 luabind-fix 补丁的 class_cache.hpp）。
set -u
REPO=/root/p3-test/p3-934bf0d.vl0cYiyD/zhengtugf-p3-934bf0d
SCRIPT=$REPO/src/server/script
BOOST=/root/p3-test/p3-scenes-boost-fix/include
DEPS=/root/p3-test/p3-lua-deps/lib
FLAGS="-m32 -O2 -D_USE_EPOLL_ -D_USE_CMD_NAMESPACE -D_ENCDEC_MSG"
export LIBRARY_PATH=/root/p3-test/p3-lua-deps/lib:/usr/local/lib
export PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin

echo "########## 1. 解包 luabind 源码 ##########"
rm -rf /root/luabind-src && mkdir -p /root/luabind-src
tar xzf /root/luabind-src.tgz -C /root/luabind-src
ls /root/luabind-src/src/*.cpp | wc -l

echo "########## 2. 编 libluabind.a（m32, Boost 1.34.1） ##########"
mkdir -p /root/luabind-src/obj
rc=0
for f in /root/luabind-src/src/*.cpp; do
  o=/root/luabind-src/obj/$(basename "$f" .cpp).o
  if ! g++ $FLAGS -c "$f" -o "$o" \
       -I"$SCRIPT" -I"$BOOST" -I"$SCRIPT/luabind" \
       -D_GNU_SOURCE -D_REENTRANT 2> "/root/luabind-src/obj/$(basename "$f").log"; then
    echo "  [FAIL] $(basename "$f")"; grep -m3 -i error "/root/luabind-src/obj/$(basename "$f").log"; rc=1; break
  fi
done
[ $rc -ne 0 ] && exit 1
ls /root/luabind-src/obj/*.o | wc -l
ar rcs /root/luabind-src/libluabind.a /root/luabind-src/obj/*.o
ls -l /root/luabind-src/libluabind.a

echo "########## 3. 替换 p3-lua-deps/lib/libluabind.a ##########"
[ -f "$DEPS/libluabind.a.bak-133" ] || cp "$DEPS/libluabind.a" "$DEPS/libluabind.a.bak-133"
cp /root/luabind-src/libluabind.a "$DEPS/libluabind.a"

echo "########## 4. 全量清编 ScenesServer ##########"
make -C "$REPO/src/server/ScenesServer" clean > /dev/null 2>&1
export ZEBRA_BOOST_INCLUDE="$BOOST"
make -C "$REPO/src/server/ScenesServer" -j4 CC=gcc CXX=g++ "ZEBRA_COMPILE_OPT=$FLAGS" > /root/luabind_scenes_make.log 2>&1
rc=$?
echo "  make rc=$rc"
[ $rc -ne 0 ] && { grep -n -i 'error\|\*\*\*' /root/luabind_scenes_make.log | head -15; exit 1; }
ls -l "$REPO/src/server/ScenesServer/ScenesServer"

echo "########## 5. 部署 ##########"
cp "$REPO/src/server/ScenesServer/ScenesServer" "$REPO/build/p3-native/ScenesServer"
chmod +x "$REPO/build/p3-native/ScenesServer"
ls -l "$REPO/build/p3-native/ScenesServer"
echo "########## LUABIND REBUILD DONE ##########"
