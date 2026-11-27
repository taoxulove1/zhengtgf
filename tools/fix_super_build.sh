#!/bin/bash
# fix_super_build.sh —— 只重编 SuperServer（修补 initServerSequence 静态初始化顺序）
# 注意：patch 后的构建脚本必须放在仓库 tools/ 下，build_native_server.py 用
#       Path(__file__).parents[1] 定位 ROOT，放到 /root 下会算出 ROOT=/ 而报 "Missing source Makefile"。
set -u
P=/root/p3-test/p3-934bf0d.vl0cYiyD/zhengtugf-p3-934bf0d
cd "$P" || exit 1

python3 - <<'PYEOF'
src = "/root/p3-test/p3-934bf0d.vl0cYiyD/zhengtugf-p3-934bf0d/tools/build_native_server.py"
dst = "/root/p3-test/p3-934bf0d.vl0cYiyD/zhengtugf-p3-934bf0d/tools/build_super_only.py"
d = open(src, encoding="utf-8").read()
old = 'SERVICES = (\n    "SuperServer", "FLServer", "RecordServer", "BillServer",\n    "MiniServer", "SessionServer", "ScenesServer", "GatewayServer",\n)'
new = 'SERVICES = (\n    "SuperServer",\n)'
if old in d:
    d = d.replace(old, new, 1)
    print("SERVICES 已裁剪为只含 SuperServer")
else:
    print("未匹配 SERVICES，保持原样")
open(dst, "w", encoding="utf-8").write(d)
print("输出:", dst)
PYEOF

export LIBRARY_PATH=/root/p3-test/p3-lua-deps/lib:/usr/local/lib
export LD_LIBRARY_PATH="$LIBRARY_PATH"
nohup python3 "$P/tools/build_super_only.py" --jobs 2 > /root/zt/log/native/build_super.log 2>&1 &
echo "super build pid=$!"
