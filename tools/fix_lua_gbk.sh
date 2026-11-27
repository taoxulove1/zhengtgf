#!/bin/bash
# fix_lua_gbk.sh —— 给 Lua 5.1 词法器打 GBK 标识符补丁，从根源解决 newquest 脚本的中文标识符
#
# 证据链：
#   npc_848.lua 反复报 'unexpected symbol near ?' / '<name>' expected，
#   涉及 local menu_848_领取装备、function 领取装备()、发送给:玩家(...) 这类写法——
#   这套脚本本来就是给「支持 GBK 标识符的 Lua」写的，属于合法的私有 Lua 变体语法。
#   正则清洗标识符治标不治本（方法调用链、混合标识符层出不穷），改为给词法器开口子：
#   字节 >= 0x81 一律当作标识符字符，GBK 双字节按整字消费。
#
# 步骤：api.github.com 拉 lua v5.1.1 -> 补 llex.c -> 重编 liblua.a(m32)
#       -> 替换 p3-lua-deps/lib/liblua.a -> 重链 ScenesServer -> 还原 .bak-cjk
set -u
REPO=/root/p3-test/p3-934bf0d.vl0cYiyD/zhengtugf-p3-934bf0d
DEPS=/root/p3-test/p3-lua-deps/lib
FLAGS="-m32 -O2"
export LIBRARY_PATH=$DEPS:/usr/local/lib
export PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin

echo "########## 1. 解包 Lua 5.1.1 ##########"
rm -rf /root/lua51 && mkdir -p /root/lua51
tar xzf /root/lua51.tgz -C /root/lua51
LUA=$(ls -d /root/lua51/lua-lua-*/)
echo "  $LUA"

echo "########## 2. 补 llex.c（GBK 标识符） ##########"
python3 - "$LUA" <<'PY'
import sys, re
p = sys.argv[1].rstrip("/") + "/llex.c"
d = open(p, "rb").read()
if b"P3-FIX: GBK identifiers" in d:
    print("  已打过补丁")
    raise SystemExit(0)
# 入口条件：标识符可以从 GBK 字节开始（EOZ=-1 必须排除，否则 EOF 处 unsigned 变 0xFF 误判）
d2, n1 = re.subn(
    rb"else if \(isalpha\(ls->current\) \|\| ls->current == '_'\) \{",
    b"else if (isalpha(ls->current) || ls->current == '_' || (ls->current != EOZ && (unsigned char)ls->current >= 0x81)) { /* P3-FIX: GBK identifiers */",
    d)
# 扫描循环：GBK 双字节整字消费（尾字节 0x40-0xFE 都有，不能按 >=0x81 判断）
d3, n2 = re.subn(
    rb"do \{\r?\n\s*save_and_next\(ls\);\r?\n\s*\} while \(isalnum\(ls->current\) \|\| ls->current == '_'\);",
    b"do {\n"
    b"            if (ls->current != EOZ && (unsigned char)ls->current >= 0x81 && (unsigned char)ls->current <= 0xFE) {\n"
    b"              save_and_next(ls); /* P3-FIX: GBK lead */\n"
    b"              save_and_next(ls); /* P3-FIX: GBK trail (0x40-0xFE, consume unconditionally) */\n"
    b"            } else {\n"
    b"              save_and_next(ls);\n"
    b"            }\n"
    b"          } while (ls->current != EOZ && (isalnum(ls->current) || ls->current == '_' || (unsigned char)ls->current >= 0x81));",
    d2)
if n1 != 1 or n2 != 1:
    raise SystemExit("补丁点没对上: entry=%d loop=%d" % (n1, n2))
open(p, "wb").write(d3)
print("  llex.c 已补丁 (entry/loop 各 1 处)")
PY

echo "########## 3. 重编 liblua.a (m32) ##########"
CORE="lapi.c lcode.c ldebug.c ldo.c ldump.c lfunc.c lgc.c llex.c lmem.c lobject.c lopcodes.c lparser.c lstate.c lstring.c ltable.c ltm.c lundump.c lvm.c lzio.c"
LIBS="lauxlib.c lbaselib.c ldblib.c liolib.c lmathlib.c loslib.c ltablib.c lstrlib.c loadlib.c linit.c"
mkdir -p /root/lua51/obj && rm -f /root/lua51/obj/*.o
cd "$LUA" || exit 1
for f in $CORE $LIBS; do
  gcc $FLAGS -c "$f" -o "/root/lua51/obj/${f%.c}.o" -D_GNU_SOURCE 2> "/root/lua51/obj/${f%.c}.err" \
    || { echo "  [FAIL] $f"; head -5 "/root/lua51/obj/${f%.c}.err"; exit 1; }
done
ls /root/lua51/obj/*.o | wc -l
ar rcs /root/lua51/liblua.a /root/lua51/obj/*.o
ls -l /root/lua51/liblua.a

echo "########## 4. 替换 liblua.a ##########"
[ -f "$DEPS/liblua.a.bak-orig" ] || cp "$DEPS/liblua.a" "$DEPS/liblua.a.bak-orig"
cp /root/lua51/liblua.a "$DEPS/liblua.a"

echo "########## 5. 还原被清洗过的脚本 ##########"
n=0
for b in $(find /root/zt/server/newquest -name '*.bak-cjk' 2>/dev/null); do
  cp "$b" "${b%.bak-cjk}"; n=$((n+1))
done
echo "  还原 $n 个脚本"

echo "########## 6. 重链 ScenesServer ##########"
rm -f "$REPO/src/server/ScenesServer/ScenesServer"
make -C "$REPO/src/server/ScenesServer" -j4 CC=gcc CXX=g++ \
  "ZEBRA_COMPILE_OPT=-m32 -O2 -D_USE_EPOLL_ -D_USE_CMD_NAMESPACE -D_ENCDEC_MSG" \
  > /root/lua_relink.log 2>&1
rc=$?
echo "  make rc=$rc"
[ $rc -ne 0 ] && { grep -n -i 'error\|\*\*\*\|undefined' /root/lua_relink.log | head -15; exit 1; }
cp "$REPO/src/server/ScenesServer/ScenesServer" "$REPO/build/p3-native/ScenesServer"
chmod +x "$REPO/build/p3-native/ScenesServer"
ls -l "$REPO/build/p3-native/ScenesServer"

echo "########## 7. 编 lua 解释器并批量校验 newquest 全部脚本 ##########"
cd "$LUA" || exit 1
gcc $FLAGS -o /root/lua51/lua lua.c /root/lua51/obj/*.o -lm 2>/dev/null
python3 - <<'PY'
import os, subprocess
bad = []
for dp, _d, fs in os.walk("/root/zt/server/newquest"):
    for fn in fs:
        if fn.endswith(".lua"):
            p = os.path.join(dp, fn)
            r = subprocess.run(["/root/lua51/lua", "-e",
                "local f,e=loadfile('%s'); print(f and 'OK' or e)" % p],
                stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
            out = r.stdout.decode("gbk", "replace").strip()
            if not out.endswith("OK"):
                bad.append((p, out))
print("语法错误文件数:", len(bad))
for p, e in bad[:12]:
    print(" ", p, "->", e[:120])
PY
echo "########## LUA GBK DONE ##########"
