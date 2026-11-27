#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""apply_repo_patches.py —— 把服务器侧已验证的两组源码补丁同步进仓库 src/server。

补丁 1（ScenesServer.h）：ScenesService 构造函数补齐未初始化成员。
    根因见 docs/evidence/20261128-native-stack-6010.md §2：
    han_biaoche 等成员从未初始化，MSVC new 常拿零页所以 Win 版碰巧不炸，
    glibc 给脏内存 -> SceneTimeTick::run() 第一个 tick 解引用野指针 segfault。

补丁 2（base/zDatabaseManager.h）：refresh(const char*) 打开文件失败时
    ret=true 降级为告警（TABLE-MISSING skip），不再让 loadAllBM() 一张缺表就整体失败。
    这套源码比服务器 map/ 数据新 45 张扩展表，缺表属正常。

两个补丁都已幂等（重复执行跳过）。
"""
import re
import sys

ROOT = r"E:\源端\zhengtugf\src\server"


def patch_scenes_header():
    p = ROOT + r"\ScenesServer\ScenesServer.h"
    d = open(p, "rb").read()
    if b"P3-FIX: uninit members" in d:
        print("[skip] ScenesServer.h 已有补丁")
        return
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
        b"\t\t\t/* ------------------------------------------------------------- */"
    )
    pat = re.compile(rb"writeBackTimer = 0;\r?\n\s*\r?\n\s*taskPool = NULL;")
    m = pat.search(d)
    if not m:
        sys.exit("[ERR] ScenesServer.h 构造函数没匹配到")
    open(p, "wb").write(d[:m.start()] + block + d[m.end():])
    print("[ok] ScenesServer.h 构造函数补齐")


def patch_dbm_header():
    p = ROOT + r"\base\zDatabaseManager.h"
    d = open(p, "rb").read()
    if b"TABLE-MISSING" in d:
        print("[skip] zDatabaseManager.h 已有补丁")
        return
    pat = re.compile(rb'Zebra::logger->error\("[^"\r\n]* %s",filename\);')
    m = pat.search(d)
    if not m:
        sys.exit("[ERR] zDatabaseManager.h 打开失败分支没匹配到")
    new = (b'Zebra::logger->error("TABLE-MISSING skip %s",filename); ret=true; '
           b"/* P3-FIX: missing tbl tolerated */")
    open(p, "wb").write(d[:m.start()] + new + d[m.end():])
    print("[ok] zDatabaseManager.h 缺表降级")


def main():
    patch_scenes_header()
    patch_dbm_header()


if __name__ == "__main__":
    main()
