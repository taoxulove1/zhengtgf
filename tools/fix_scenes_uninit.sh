#!/bin/bash
# fix_scenes_uninit.sh —— 修原生 ScenesServer 初始化期 segfault（6010 的真正断点）
#
# 证据链：
#   1) tools/native_boot.sh：整栈原生按 FL->Super->... 顺序启动后，ScenesServer 日志出现
#        "初始化 场景服务器:6010 成功" —— 6010 能 bind，端口不是问题。
#   2) 紧接着 ScenesServer Segfault。tools/scenes_gdb.sh 抓到的栈：
#        #0 SkillStatusManager::clearActiveSkillStatusOnlyUseToStatus48()
#        #1 SceneTimeTick::run()
#      也就是首个 tick 就崩，崩在 skillStatusM 的 this 上（ebx/this 是垃圾值）。
#   3) 调用点 TimeTick.cpp:114 的写法是
#        if (ScenesService::getInstance().han_biaoche != NULL)
#            han_biaoche->skillStatusM.clearActiveSkillStatusOnlyUseToStatus48();
#      守卫是 != NULL，所以 han_biaoche 本身不是 NULL，是**野指针**。
#   4) ScenesServer.h:165 的构造函数只写了 writeBackTimer 和 taskPool：
#        ScenesService() : zSubNetService("场景服务器", SCENESSERVER)
#        { writeBackTimer = 0; taskPool = NULL; }
#      han_biaoche / han_biaoche2 / chu_biaoche / chu_biaoche2 / migong[] / tianxia[] /
#      fanwaigua / countryPower[] / double_exp 等成员**从未初始化**。
#      MSVC 的 operator new 常拿到零页，Windows 版因此碰巧不炸；glibc 的 operator new 给脏内存，
#      第一个 tick 就解引用野指针。这是纯 native 构建暴露出来的既有 bug，不是移植引入的。
#
# 处理：构造函数里把这些成员显式清零/置 NULL，然后只重编 ScenesServer。
set -u
SRC=/root/p3-test/p3-934bf0d.vl0cYiyD/zhengtugf-p3-934bf0d/src/server/ScenesServer
NB=/root/p3-test/p3-934bf0d.vl0cYiyD/zhengtugf-p3-934bf0d/build/p3-native
FLAGS="-m32 -O2 -D_USE_EPOLL_ -D_USE_CMD_NAMESPACE -D_ENCDEC_MSG"
export LIBRARY_PATH=/root/p3-test/p3-lua-deps/lib:/usr/local/lib
export PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
# ScenesServer 的 script.cpp 走 luabind，luabind 的预处理期 BOOST_PP_ITERATION_* 宏
# 必须用随源码带的 Boost 1.34.1；系统 Boost(CentOS7=1.53) 会让它报
# "missing binary operator before token '('"（#elif BOOST_PP_ITERATION_FLAGS() == 1 处）。
# 这个 include 目录必须显式给，否则只有 script.o 编不过、其余 .o 全正常。
export ZEBRA_BOOST_INCLUDE=/root/p3-test/p3-scenes-boost-fix/include

echo "########## 1. 打补丁 ScenesServer.h ##########"
[ -f "$SRC/ScenesServer.h.bak-uninit" ] || cp "$SRC/ScenesServer.h" "$SRC/ScenesServer.h.bak-uninit"
python3 - <<'PY'
import re
p = "/root/p3-test/p3-934bf0d.vl0cYiyD/zhengtugf-p3-934bf0d/src/server/ScenesServer/ScenesServer.h"
d = open(p, "rb").read()
if b"P3-FIX: uninit members" in d:
    print("  已打过补丁，跳过")
    raise SystemExit(0)
block = (
    b"writeBackTimer = 0;\r\n"
    b"\r\n"
    b"\t\t\ttaskPool = NULL;\r\n"
    b"\r\n"
    b"\t\t\t/* P3-FIX: uninit members ----------------------------------------\r\n"
    b"\t\t\t * \xe4\xbb\xa5\xe4\xb8\x8b\xe6\x88\x90\xe5\x91\x98\xe5\x8e\x9f\xe6\x9e\x84\xe9\x80\xa0\xe5\x87\xbd\xe6\x95\xb0\xe9\x87\x8c\xe4\xbb\x8e\xe6\x9c\xaa\xe5\x88\x9d\xe5\xa7\x8b\xe5\x8c\x96\xe3\x80\x82\r\n"
    b"\t\t\t * MSVC \xe7\x9a\x84 new \xe5\xb8\xb8\xe6\x8b\xbf\xe5\x88\xb0\xe9\x9b\xb6\xe9\xa1\xb5\xe6\x89\x80\xe4\xbb\xa5 Win \xe7\x89\x88\xe7\xa2\xb0\xe5\xb7\xa7\xe4\xb8\x8d\xe7\x82\xb8\xef\xbc\x9b\r\n"
    b"\t\t\t * glibc \xe7\x9a\x84 operator new \xe7\xbb\x99\xe8\x84\x8f\xe5\x86\x85\xe5\xad\x98\xef\xbc\x8chan_biaoche \xe6\x88\x90\xe4\xb8\xba\xe9\x87\x8e\xe6\x8c\x87\xe9\x92\x88\xef\xbc\x8c\r\n"
    b"\t\t\t * SceneTimeTick::run() \xe7\xac\xac\xe4\xb8\x80\xe4\xb8\xaa tick \xe5\xb0\xb1\xe5\xb4\xa9\xe3\x80\x82 */\r\n"
    b"\t\t\tmemset(tianxia, 0, sizeof(tianxia));\r\n"
    b"\t\t\tfanwaigua = false;\r\n"
    b"\t\t\tfanwaiguarongcuo = 0;\r\n"
    b"\t\t\tmemset(migong, 0, sizeof(migong));\r\n"
    b"\t\t\than_biaoche = NULL;\r\n"
    b"\t\t\than_biaoche2 = NULL;\r\n"
    b"\t\t\tchu_biaoche = NULL;\r\n"
    b"\t\t\tchu_biaoche2 = NULL;\r\n"
    b"\t\t\thanbiaoche_type = 0;\r\n"
    b"\t\t\tchubiaoche_type = 0;\r\n"
    b"\t\t\tdouble_exp = 1;\r\n"
    b"\t\t\tdouble_drop = false;\r\n"
    b"\t\t\tlevelDoubleDrop = 0;\r\n"
    b"\t\t\tmemset(countryPower, 0, sizeof(countryPower));\r\n"
    b"\t\t\t/* ------------------------------------------------------------- */\r\n"
)
pat = re.compile(rb"writeBackTimer = 0;\r?\n\s*\r?\n\s*taskPool = NULL;")
m = pat.search(d)
if not m:
    raise SystemExit("  [ERR] 没匹配到构造函数原体，放弃")
d2 = d[:m.start()] + block.rstrip(b"\r\n") + d[m.end():]
open(p, "wb").write(d2)
print("  构造函数已补初始化，", len(d2) - len(d), "字节增量")
PY

echo "########## 2. 重编 ScenesServer（只这一个服务） ##########"
make -C "$SRC" -j4 CC=gcc CXX=g++ "ZEBRA_COMPILE_OPT=$FLAGS" 2>&1 | tail -25
RC=${PIPESTATUS[0]}
echo "  make rc=$RC"
if [ ! -x "$SRC/ScenesServer" ]; then echo "  [ERR] 产物缺失"; exit 1; fi
ls -l "$SRC/ScenesServer"

echo "########## 3. 替换 build 产物 ##########"
[ -f "$NB/ScenesServer.bak-uninit" ] || cp "$NB/ScenesServer" "$NB/ScenesServer.bak-uninit"
cp "$SRC/ScenesServer" "$NB/ScenesServer"
chmod +x "$NB/ScenesServer"
ls -l "$NB/ScenesServer"
echo "########## REBUILD DONE ##########"
