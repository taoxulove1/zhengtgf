# -*- coding: utf-8 -*-
"""patchupdate.exe 域名/IP 全量 patch（幂等，可重复执行）。

官方 patchupdate.exe（源1，3.1.46.0 怀旧版）把列表主机写成四级 fallback 链，
按 test 优先级：
    1. 配置数组 0x46ade8[i*0x41c]  —— 条目0 = 211.159.201.33（官方内网测试机）
                                      条目1 = 172.17.102.60（官方内网测试机）
    2. 单串       0x46c278        —— 211.159.201.33
    3. 数组       0x46ba40        —— srvlistall.ztgame.com.cn
    4. 最终 fallback 0x46c694     —— srvlistall.ztgame.com.cn

三个候选全是 14 字符，与 101.33.242.142 等长，可无损替换。
patchsetup.ini 根本没有被读取列表主机的逻辑（update/server/servername 键均无效），
唯一正道就是改二进制。

用法： python tools/patch_patchupdate.py            # 从原版备份重新生成并覆盖
      python tools/patch_patchupdate.py --verify    # 只校验当前状态
"""
import os
import sys

SRC1 = r"E:\源端\源1"
ORIG = r"E:\源端\zhengtugf\captures\patchupdate.exe.bak-orig"
EXE = os.path.join(SRC1, "patchupdate.exe")
NEW = b"101.33.242.142"

OLDS = [b"srvlistall.ztgame.com.cn\x00", b"211.159.201.33\x00", b"172.17.102.60\x00"]


def patch():
    data = bytearray(open(ORIG, "rb").read())
    total = 0
    for old in OLDS:
        new = NEW + b"\x00" * (len(old) - len(NEW))
        i = 0
        while True:
            i = data.find(old, i)
            if i < 0:
                break
            data[i:i + len(old)] = new
            total += 1
            i += len(old)
    tmp = EXE + ".tmp"
    open(tmp, "wb").write(bytes(data))
    os.replace(tmp, EXE)
    d = open(EXE, "rb").read()
    ok = d.find(b"srvlistall") < 0 and d.count(NEW) == total
    print("patched %d sites, verify=%s, size=%d" % (total, ok, len(d)))
    return ok


def verify():
    d = open(EXE, "rb").read()
    n = d.count(NEW)
    ok = d.find(b"srvlistall") < 0 and n >= 6
    print("current: new-hits=%d, old-gone=%s -> %s" % (n, d.find(b"srvlistall") < 0, "OK" if ok else "NEED-PATCH"))
    return ok


if __name__ == "__main__":
    sys.exit(0 if (verify() if "--verify" in sys.argv else patch()) else 1)
