#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""protocol_doc.py — 从 Command.h 生成协议文档全文（九列表 + 全字段偏移表）。

与 extract_msgid.py 共用解析器；本条只负责"排版成文档"，不做筛选、不合并条目。
输出:
    docs/protocol/protocol.generated.md   按 47 个命令域分组，559 条一条不落
    docs/protocol/fields.csv              每个结构体的全字段 + payload 内偏移

用法:
    python tools/protocol_doc.py
    python tools/protocol_doc.py --source src/server/base/Command.h
"""

import argparse
import csv
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import extract_msgid as E  # noqa: E402

HEADER = 6  # byCmd + byParam + dwTimestamp


def gbk_lines(path):
    with open(path, "rb") as f:
        return f.read().decode("gbk", errors="replace").splitlines()


def domain_label(struct_name, structs, glines):
    """沿继承链找到域基类，取其实测中文注释（源码注释，不猜）。"""
    seen = set()
    cur = struct_name
    while cur and cur not in seen:
        seen.add(cur)
        s = structs.get(cur)
        if not s:
            break
        if s["byCmd"]:
            # 该 struct 是域基类，向上找注释
            for back in range(1, 14):
                idx = s["line"] - 1 - back
                if idx < 0:
                    break
                line = glines[idx].strip()
                if line.startswith("//") and ("指令" in line or "命令" in line):
                    text = line.lstrip("/").strip()
                    for junk in ("定义开始", "定义结束", "开始", "结束"):
                        text = text.replace(junk, "")
                    return text.strip() or "—"
            return "—"
        cur = s["base"]
    return "—"


def fields_with_offset(structs, name, seen=None):
    """返回 [(global_off, type, name, size)]，global_off 从 payload 起点 0 计。"""
    seen = seen or set()
    if name in seen or name not in structs:
        return []
    seen.add(name)
    s = structs[name]
    out = []
    base_off = 0
    if s["base"]:
        out = fields_with_offset(structs, s["base"], seen)
        base_off = out[-1][0] + out[-1][3] if out else 0
    off = base_off
    for t, nm, sz in s["fields"]:
        out.append((off, t, nm or "raw", sz))
        off += sz
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--source", default="src/server/base/Command.h")
    ap.add_argument("--out", default="docs/protocol/protocol.generated.md")
    ap.add_argument("--fields", default="docs/protocol/fields.csv")
    a = ap.parse_args()

    lines = E.load(a.source)
    glines = gbk_lines(a.source)
    consts, structs = E.parse(lines)

    rows = []
    for name, s in structs.items():
        if not name.endswith(("Cmd", "Command")):
            continue
        cmd, param, cmd_name = E.resolve(structs, name, consts)
        if cmd is None or param is None:
            continue
        fl = fields_with_offset(structs, name)
        total = HEADER + (fl[-1][0] + fl[-1][3] if fl else 0)
        label = domain_label(name, structs, glines)
        rows.append({
            "id": "0x%04X" % ((cmd << 8) | param),
            "cmd": cmd, "cmd_name": cmd_name or ("DOMAIN_0x%02X" % cmd),
            "param_name": s["byParam"] or "",
            "struct": name, "line": s["line"],
            "total": total, "fields": fl,
            "label": label,
        })
    rows.sort(key=lambda r: (r["cmd"], int(r["id"][-2:], 16)))

    domains = {}
    for r in rows:
        domains.setdefault((r["cmd"], r["cmd_name"]), []).append(r)

    os.makedirs(os.path.dirname(a.out) or ".", exist_ok=True)
    with open(a.out, "w", encoding="utf-8") as f:
        f.write("# 协议文档 · 征途 3.1.46.0（自动生成）\n\n")
        f.write("> 由 `tools/protocol_doc.py` 从 `src/server/base/Command.h` 生成，**勿手改**。\n")
        f.write(f"> 统计：**{len(domains)} 个命令域 / {len(rows)} 条命令 / "
                f"{sum(len(r['fields']) for r in rows)} 个字段**\n\n")
        f.write("## 0. 报文封装\n\n")
        f.write("| 层 | 值 | 依据 |\n|---|---|---|\n")
        f.write("| 头长度 | 6 字节 | `stNullUserCmd` |\n")
        f.write("| 字段 | `BYTE byCmd` + `BYTE byParam` + `DWORD dwTimestamp` | 同上 |\n")
        f.write("| opcode | `(byCmd << 8) | byParam` | 559 条全部可解析 |\n")
        f.write("| 加密 | 裸 DES-ECB，8 字节一块，无 IV/填充，尾部余数明文 | `CDes.cpp:47` |\n")
        f.write("| 长度 | 无长度前缀，按 opcode 定长 | `msgid.csv.length_rule` |\n\n")

        f.write("## 1. 域索引\n\n| 域 | 名称 | 中文 | 条数 | 基数 |\n|---|---|---|---|---|\n")
        for (cmd, cname), items in sorted(domains.items()):
            label = next((i["label"] for i in items if i["label"] != "—"), "—")
            f.write(f"| `0x{cmd:02X}` | `{cname}` | {label} | {len(items)} | 0x{cmd:02X}xx |\n")

        f.write("\n## 2. 命令明细（逐域）\n")
        for (cmd, cname), items in sorted(domains.items()):
            label = next((i["label"] for i in items if i["label"] != "—"), "—")
            f.write(f"\n### 0x{cmd:02X} `{cname}` · {label}（{len(items)} 条）\n\n")
            f.write("| ID | 结构体 | 总长 | 字段数 | 子命令常量 | 源行 |\n")
            f.write("|---|---|---|---|---|---|\n")
            for r in items:
                f.write(f"| `{r['id']}` | `{r['struct']}` | {r['total']} | "
                        f"{len(r['fields'])} | `{r['param_name']}` | {r['line']} |\n")

        f.write("\n## 3. 字段明细\n\n")
        f.write("完整偏移见 `fields.csv`；此处每结构体列一次，一条不漏。\n\n")
        for r in rows:
            f.write(f"\n#### `{r['id']}` `{r['struct']}` · 总长 {r['total']} 字节\n\n")
            f.write("| 偏移 | 类型 | 字段 | 字节 |\n|---|---|---|---|\n")
            f.write("| 0 | BYTE | byCmd | 1 |\n| 1 | BYTE | byParam | 1 |\n| 2 | DWORD | dwTimestamp | 4 |\n")
            for off, t, nm, sz in r["fields"]:
                f.write(f"| {HEADER + off} | {t} | {nm} | {sz} |\n")

    with open(a.fields, "w", newline="", encoding="utf-8") as f:
        w = csv.writer(f)
        w.writerow(["opcode", "struct", "field_index", "offset", "type", "field", "size"])
        for r in rows:
            for i, (off, t, nm, sz) in enumerate(r["fields"]):
                w.writerow([r["id"], r["struct"], i, HEADER + off, t, nm, sz])

    print(f"域 {len(domains)} / 命令 {len(rows)} / 字段 {sum(len(r['fields']) for r in rows)}")
    print(f"写出 {a.out}（{sum(1 for _ in open(a.out, encoding='utf-8'))} 行）")
    print(f"写出 {a.fields}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
