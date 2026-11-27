#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""sanitize_lua_ident.py —— 把 newquest lua 里标识符位置的 GBK 尾巴换成 ASCII。

背景：native ScenesServer 的 Lua 词法器不认 GBK 中文标识符
（npc_848.lua:18 `local menu_848_领取装备 = [[` -> unexpected symbol near '?'）。
字符串里的中文（AddTalk 文案等）不能动，只处理「标识符中段/尾段」的 GBK 字节。

规则：仅当 GBK 串紧跟在 [A-Za-z0-9_] 后面、且后面跟的又是标识符结束符
（空白、=、(、,、) 、行尾）时才替换。同名 GBK 串在同一文件里替换成同一个 ASCII 名，
保证定义和引用一致。替换前自动备份 .bak-cjk。
"""
import os
import re
import sys

ROOT = "/root/zt/server/newquest"
BAK_SUFFIX = ".bak-cjk"

GBK_SEQ = rb"[\x81-\xfe][\x40-\xfe]"
# 标识符里的 GBK 段：前面是 ASCII 标识符字符，GBK 段后面必须跟非 GBK 字节
IDENT_TAIL = re.compile(
    rb"([A-Za-z0-9_])((?:" + GBK_SEQ + rb")+)(?=$|[^" rb"\x40-\xfe" rb"])"
)
# 中文开头的标识符（含 GBK 前缀+ASCII 后缀的混合形式）：
# 前面是行首/空白/(/[,,=]，后面跟标识符字符或边界符。
# 字符串里的中文前面一定是引号或正文中其它 GBK 字节，不会命中。
IDENT_STANDALONE = re.compile(
    rb"(^|[\s(,=])((?:" + GBK_SEQ + rb")+)(?=[A-Za-z0-9_\s=(),.)])"
)


def sanitize(path):
    raw = open(path, "rb").read()
    mapping = {}
    repl_counter = 0

    def make_sub():
        def sub(m):
            nonlocal repl_counter
            tail = m.group(2)
            if tail not in mapping:
                repl_counter += 1
                mapping[tail] = ("_cjk%d" % repl_counter).encode()
            return m.group(1) + mapping[tail]
        return sub

    def sub_tail(m):
        nonlocal repl_counter
        tail = m.group(2)
        if tail not in mapping:
            repl_counter += 1
            mapping[tail] = ("_cjk%d" % repl_counter).encode()
        return m.group(1) + mapping[tail]

    new = IDENT_TAIL.sub(sub_tail, raw)
    new = IDENT_STANDALONE.sub(make_sub(), new)
    if new != raw:
        bak = path + BAK_SUFFIX
        if not os.path.exists(bak):
            open(bak, "wb").write(raw)
        open(path, "wb").write(new)
        return len(mapping)
    return 0


def main():
    changed = []
    for dirpath, _dirs, files in os.walk(ROOT):
        for fn in files:
            if fn.endswith(".lua"):
                p = os.path.join(dirpath, fn)
                n = sanitize(p)
                if n:
                    changed.append((p, n))
    print("改动文件数:", len(changed))
    for p, n in changed:
        print("  %s : %d 个中文标识符" % (p, n))


if __name__ == "__main__":
    main()
