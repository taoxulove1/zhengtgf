#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""extract_msgid.py — 从服务端 Command.h 提取协议命令表。

Command.h 的命令模型（实测）：
    const BYTE NULL_USERCMD_PARA = 0;
    struct stNullUserCmd {
        union { struct { BYTE byCmd; BYTE byParam; };
                struct { BYTE byCmdType; BYTE byParameterType; }; };
        DWORD dwTimestamp;          // 固定 6 字节头
    };
    struct stLogonUserCmd : public stNullUserCmd { stLogonUserCmd(){ byCmd = LOGON_USERCMD; } };
    struct stUserVerifyVerCmd : public stLogonUserCmd { stUserVerifyVerCmd(){ byParam = USER_VERIFY_VER_PARA; } };

于是 opcode = (byCmd << 8) | byParam，2 字节；包体前 6 字节为头。

用法:
    python tools/extract_msgid.py --source src/server/base/Command.h \
        --out docs/protocol/msgid.csv --schema docs/protocol/schema/commands.h
"""

import argparse
import csv
import os
import re
import sys

# GBK 源码按 latin-1 读，避免解码中断；只关心 ASCII 部分
def load(path):
    with open(path, "rb") as f:
        return f.read().decode("latin-1").splitlines()


CONST_PATTERNS = [
    re.compile(r"^\s*#define\s+([A-Z][A-Z0-9_]{2,40})\s+(\d+)\s*$"),
    re.compile(r"^\s*const\s+BYTE\s+([A-Z][A-Z0-9_]{2,40})\s*=\s*(\d+)\s*;"),
]
STRUCT_PAT = re.compile(r"^\s*struct\s+(st[A-Za-z0-9_]+)\s*(?::\s*public\s+(st[A-Za-z0-9_]+))?\s*\{?")
ASSIGN_PAT = re.compile(r"\bby(Cmd|Param)\s*=\s*([A-Z][A-Z0-9_]*)\s*;")
FIELD_PAT = re.compile(r"^\s*(BYTE|WORD|DWORD|QWORD|int|unsigned\s+char|char|float|double|short|unsigned\s+int|long)\s+([A-Za-z_][A-Za-z0-9_]*)\s*(\[[^\]]*\])?\s*;")
CHARARR_PAT = re.compile(r"^\s*char\s+[A-Za-z_][A-Za-z0-9_]*\s*\[\s*(\d+)\s*\]\s*;")

TYPE_SIZE = {
    "BYTE": 1, "unsigned char": 1, "char": 1,
    "WORD": 2, "short": 2,
    "DWORD": 4, "int": 4, "unsigned int": 4, "long": 4, "float": 4,
    "QWORD": 8, "double": 8,
}

HEADER_BYTES = 6  # byCmd + byParam + dwTimestamp


def parse(lines):
    consts = {}
    structs = {}   # name -> {base, byCmd, byParam, fields:[(type,name,size)], line}
    cur = None
    depth = 0

    for i, raw in enumerate(lines, 1):
        line = raw.split("//")[0]

        for p in CONST_PATTERNS:
            m = p.match(line)
            if m:
                consts[m.group(1)] = int(m.group(2))

        m = STRUCT_PAT.match(line)
        if m:
            cur = {"name": m.group(1), "base": m.group(2), "byCmd": None,
                   "byParam": None, "fields": [], "line": i, "depth": line.count("{") or 1}
            structs[cur["name"]] = cur
            depth = line.count("{") - line.count("}")
            continue

        if cur is None:
            continue

        depth += line.count("{") - line.count("}")
        if depth <= 0:
            cur = None
            continue

        # 只取构造里的第一处赋值
        for m in ASSIGN_PAT.finditer(line):
            key = "byCmd" if m.group(1) == "Cmd" else "byParam"
            if cur[key] is None:
                cur[key] = m.group(2)

        m = CHARARR_PAT.match(line)
        if m:
            cur["fields"].append(("char", "", int(m.group(1))))
            continue
        m = FIELD_PAT.match(line)
        if m:
            t = re.sub(r"\s+", " ", m.group(1))
            cur["fields"].append((t, m.group(2), TYPE_SIZE.get(t, 4)))

    return consts, structs


def resolve(structs, name, consts, seen=None):
    """上溯继承链，取最近一级的域常量（byCmd）与本级子命令（byParam）。"""
    seen = seen or set()
    if name in seen or name not in structs:
        return (None, None, None)
    seen.add(name)
    s = structs[name]
    param = consts.get(s["byParam"]) if s["byParam"] else None
    if s["byCmd"] and s["byCmd"] in consts:
        return (consts[s["byCmd"]], param, s["byCmd"])
    if s["base"]:
        c, _, cn = resolve(structs, s["base"], consts, seen)
        return (c, param, cn)
    return (None, param, None)


def payload_len(structs, name, seen=None):
    """沿继承链累加字段宽度；char[N] 按 N 计。"""
    seen = seen or set()
    if name in seen or name not in structs:
        return 0, []
    seen.add(name)
    s = structs[name]
    base_len, base_fields = (0, [])
    if s["base"]:
        base_len, base_fields = payload_len(structs, s["base"], seen)
    total = base_len + sum(f[2] for f in s["fields"])
    return total, base_fields + [(f[0], f[1], f[2]) for f in s["fields"]]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--source", default="src/server/base/Command.h")
    ap.add_argument("--out", default="docs/protocol/msgid.csv")
    ap.add_argument("--schema", default="docs/protocol/schema/commands.h")
    ap.add_argument("--min-rows", type=int, default=60)
    a = ap.parse_args()

    if not os.path.exists(a.source):
        print(f"[ERR] 源文件不存在: {a.source}", file=sys.stderr)
        return 2

    lines = load(a.source)
    consts, structs = parse(lines)

    rows = []
    for name, s in structs.items():
        if not name.endswith(("Cmd", "Command")):
            continue
        cmd, param, cmd_name = resolve(structs, name, consts)
        if cmd is None or param is None:
            continue
        plen, fields = payload_len(structs, name)
        total = HEADER_BYTES + plen
        rows.append({
            "id": "0x%04X" % ((cmd << 8) | param),
            "direction": "C->S/S->C",
            "name": re.sub(r"^st|Cmd$", "", name).upper(),
            "length_rule": "fixed(%d)" % total,
            "min_len": total,
            "max_len": total,
            "phase": "P1",
            "captured": "no",
            "_struct": name,
            "_byCmd": cmd_name or "",
            "_byParam": s["byParam"] or "",
            "_line": s["line"],
            "_fields": fields,
        })

    rows.sort(key=lambda r: r["id"])
    os.makedirs(os.path.dirname(a.out) or ".", exist_ok=True)
    with open(a.out, "w", newline="", encoding="utf-8") as f:
        w = csv.writer(f)
        w.writerow(["id", "direction", "name", "length_rule", "min_len",
                    "max_len", "phase", "captured", "struct", "byCmd", "byParam", "source_line"])
        for r in rows:
            w.writerow([r["id"], r["direction"], r["name"], r["length_rule"], r["min_len"],
                        r["max_len"], r["phase"], r["captured"], r["_struct"],
                        r["_byCmd"], r["_byParam"], r["_line"]])

    # 生成 C 结构体声明骨架（供 P3 直接引用）
    os.makedirs(os.path.dirname(a.schema) or ".", exist_ok=True)
    with open(a.schema, "w", encoding="utf-8") as f:
        f.write("/* 由 tools/extract_msgid.py 从 src/server/base/Command.h 生成，勿手改 */\n")
        f.write("#pragma once\n#include <stdint.h>\n#pragma pack(push, 1)\n\n")
        f.write("typedef struct { uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp; } ZtCmdHeader;\n\n")
        for r in rows:
            f.write(f"/* {r['id']} {r['name']} total={r['min_len']} line={r['_line']} */\n")
            f.write(f"typedef struct {r['_struct']} {{\n")
            f.write("    uint8_t byCmd; uint8_t byParam; uint32_t dwTimestamp;\n")
            for t, nm, sz in r["_fields"]:
                f.write(f"    /* +{sz} */ {t} {nm or 'raw'}[{sz}];\n")
            f.write(f"}} {r['_struct']}_t;\n\n")
        f.write("#pragma pack(pop)\n")

    print(f"常量 {len(consts)} 个 / 结构体 {len(structs)} 个 / 可解析命令 {len(rows)} 条")
    print(f"写出: {a.out}")
    print(f"写出: {a.schema}")
    return 0 if len(rows) >= a.min_rows else 1


if __name__ == "__main__":
    sys.exit(main())
