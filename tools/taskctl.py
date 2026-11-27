#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""taskctl.py — 任务流引擎（唯一真源 tasks/registry.json，状态 state/queue.json）。

命令:
    plan                     打印 DAG 与各任务状态
    plan --check             DAG 合法性检查（环、悬空依赖），CI 用
    status                   状态表
    next [--json]            取下一个可执行任务（依赖全 done）
    start <id>               置 running（依赖未完成则拒绝）
    gate <id>                跑该任务门禁命令，返回其退出码
    done <id>                置 done（必须先过门禁）
    block <id> --reason "…"  置 blocked
    reopen <id>              回到 pending
    run <id>                 start →（有 ci_cmd 则执行）→ gate → done/block 一条龙

退出码: 0 正常 / 1 业务拒绝 / 2 用法或数据错误
"""

import argparse
import json
import os
import shlex
import subprocess
import sys
from datetime import datetime, timezone

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
REGISTRY = os.path.join(ROOT, "tasks", "registry.json")
STATE = os.path.join(ROOT, "state", "queue.json")

VALID_STATES = ("pending", "running", "blocked", "done")


def now():
    return datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ")


def load_registry():
    with open(REGISTRY, encoding="utf-8") as f:
        reg = json.load(f)
    ids = [t["id"] for t in reg["tasks"]]
    if len(ids) != len(set(ids)):
        raise SystemExit("[ERR] registry.json 存在重复任务 ID")
    return reg


def load_state():
    if not os.path.exists(STATE):
        return {"updated_at": now(), "tasks": {}}
    with open(STATE, encoding="utf-8") as f:
        return json.load(f)


def save_state(state):
    os.makedirs(os.path.dirname(STATE), exist_ok=True)
    state["updated_at"] = now()
    with open(STATE, "w", encoding="utf-8") as f:
        json.dump(state, f, ensure_ascii=False, indent=2)
        f.write("\n")


def task_state(state, tid):
    return state["tasks"].get(tid, {}).get("state", "pending")


def deps_ok(state, task):
    return all(task_state(state, d) == "done" for d in task.get("deps", []))


def open_deps(state, task):
    return [d for d in task.get("deps", []) if task_state(state, d) != "done"]


def find(reg, tid):
    for t in reg["tasks"]:
        if t["id"] == tid:
            return t
    return None


def check_dag(reg):
    ids = {t["id"] for t in reg["tasks"]}
    errs = []
    for t in reg["tasks"]:
        for d in t.get("deps", []):
            if d not in ids:
                errs.append(f"{t['id']} 依赖不存在: {d}")
            if d == t["id"]:
                errs.append(f"{t['id']} 自依赖")
    # 拓扑环检测
    color = {}

    def dfs(n):
        color[n] = 1
        for d in reg["tasks"][index[n]].get("deps", []):
            if color.get(d) == 1:
                errs.append(f"检测到环: {d} -> {n}")
            elif color.get(d) is None:
                dfs(d)
        color[n] = 2

    index = {t["id"]: i for i, t in enumerate(reg["tasks"])}
    for t in reg["tasks"]:
        if color.get(t["id"]) is None:
            dfs(t["id"])
    return errs


def cmd_plan(reg, state, args):
    errs = check_dag(reg)
    order = [t["id"] for t in reg["tasks"]]
    print(f"任务 DAG: {len(order)} 个节点")
    for t in reg["tasks"]:
        st = task_state(state, t["id"])
        deps = ",".join(t.get("deps", [])) or "-"
        est = t.get("est_days", [0, 0])
        mark = {"done": "[x]", "running": "[>]", "blocked": "[!]", "pending": "[ ]"}[st]
        print(f"  {mark} {t['id']:<5} {t['name']:<18} 负责人={t['owner']:<4} 依赖={deps:<10} "
              f"工时={est[0]}-{est[1]}d 状态={st}")
    if errs:
        print("\n[失败] DAG 校验:")
        for e in errs:
            print("  -", e)
        return 1
    print("\n[通过] DAG 无环、依赖闭合")
    return 0


def cmd_status(reg, state, args):
    total = len(reg["tasks"])
    cnt = {s: 0 for s in VALID_STATES}
    for t in reg["tasks"]:
        cnt[task_state(state, t["id"])] += 1
    print(f"进度: done {cnt['done']}/{total}  running {cnt['running']}  "
          f"blocked {cnt['blocked']}  pending {cnt['pending']}")
    print(f"更新时间: {state.get('updated_at', '-')}")
    for t in reg["tasks"]:
        st = task_state(state, t["id"])
        rec = state["tasks"].get(t["id"], {})
        extra = f"  门禁rc={rec.get('gate_rc')}" if "gate_rc" in rec else ""
        note = f"  note={rec['note']}" if rec.get("note") else ""
        print(f"  {t['id']:<5} {st:<8}{extra}{note}")
    nxt = [t for t in reg["tasks"] if task_state(state, t["id"]) == "pending" and deps_ok(state, t)]
    if nxt:
        print(f"\n可执行: {nxt[0]['id']} {nxt[0]['name']}")
    return 0


def cmd_next(reg, state, args):
    cands = [t for t in reg["tasks"] if task_state(state, t["id"]) == "pending" and deps_ok(state, t)]
    if not cands:
        blocked = [t for t in reg["tasks"] if task_state(state, t["id"]) in ("pending", "blocked")]
        if args.json:
            print(json.dumps({"task": None, "reason": "no-ready-task",
                              "waiting": [t["id"] for t in blocked]}, ensure_ascii=False))
        else:
            print("无可执行任务；阻塞在: " + ", ".join(t["id"] for t in blocked) if blocked else "全部完成")
        return 1
    t = cands[0]
    if args.json:
        print(json.dumps({"id": t["id"], "name": t["name"], "owner": t["owner"],
                          "gate": t.get("gate", {}).get("cmd", "")}, ensure_ascii=False))
    else:
        print(f"{t['id']}  {t['name']}  负责人={t['owner']}")
        print(f"  门禁: {t.get('gate', {}).get('cmd', '-')}")
    return 0


def cmd_start(reg, state, args):
    t = find(reg, args.id)
    if not t:
        print(f"[ERR] 未知任务 {args.id}")
        return 2
    if task_state(state, args.id) == "done":
        print(f"[跳过] {args.id} 已完成")
        return 1
    miss = open_deps(state, t)
    if miss:
        print(f"[拒绝] {args.id} 前置未完成: {', '.join(miss)}")
        return 1
    rec = state["tasks"].setdefault(args.id, {})
    rec.update({"state": "running", "started_at": now(), "actor": args.actor})
    save_state(state)
    print(f"[running] {args.id} {t['name']}  actor={args.actor}")
    return 0


def cmd_gate(reg, state, args):
    t = find(reg, args.id)
    if not t:
        print(f"[ERR] 未知任务 {args.id}")
        return 2
    gate = t.get("gate", {})
    cmd = gate.get("cmd", "")
    if not cmd:
        print(f"[门禁] {args.id} 未定义命令，视为通过")
        return 0
    print(f"[门禁] {args.id} → {cmd}")
    rc = subprocess.call(cmd, shell=True, cwd=ROOT)
    rec = state["tasks"].setdefault(args.id, {"state": task_state(state, args.id)})
    rec["gate_rc"] = rc
    rec["gate_at"] = now()
    save_state(state)
    print(f"[门禁] rc={rc}  {'通过' if rc == 0 else '未通过'}")
    return 0 if rc == 0 else 1


def cmd_done(reg, state, args):
    t = find(reg, args.id)
    if not t:
        print(f"[ERR] 未知任务 {args.id}")
        return 2
    miss = open_deps(state, t)
    if miss:
        print(f"[拒绝] {args.id} 前置未完成: {', '.join(miss)}")
        return 1
    if t.get("gate", {}).get("cmd"):
        rc = subprocess.call(t["gate"]["cmd"], shell=True, cwd=ROOT)
        if rc != 0:
            print(f"[拒绝] {args.id} 门禁未通过 rc={rc}")
            return 1
        state["tasks"].setdefault(args.id, {})["gate_rc"] = 0
    rec = state["tasks"].setdefault(args.id, {})
    rec.update({"state": "done", "finished_at": now(), "actor": args.actor})
    save_state(state)
    print(f"[done] {args.id} {t['name']}")
    return 0


def cmd_block(reg, state, args):
    if not find(reg, args.id):
        print(f"[ERR] 未知任务 {args.id}")
        return 2
    rec = state["tasks"].setdefault(args.id, {})
    rec.update({"state": "blocked", "note": args.reason, "blocked_at": now(), "actor": args.actor})
    save_state(state)
    print(f"[blocked] {args.id} 原因: {args.reason}")
    return 0


def cmd_reopen(reg, state, args):
    if not find(reg, args.id):
        print(f"[ERR] 未知任务 {args.id}")
        return 2
    rec = state["tasks"].setdefault(args.id, {})
    rec.update({"state": "pending", "note": "", "reopened_at": now(), "actor": args.actor})
    save_state(state)
    print(f"[pending] {args.id}")
    return 0


def cmd_run(reg, state, args):
    t = find(reg, args.id)
    if not t:
        print(f"[ERR] 未知任务 {args.id}")
        return 2
    if cmd_start(reg, state, args) != 0:
        return 1
    ci = t.get("ci_cmd")
    if ci:
        print(f"[执行] {ci}")
        rc = subprocess.call(ci, shell=True, cwd=ROOT)
        if rc != 0:
            cmd_block(reg, state, argparse.Namespace(id=args.id, reason=f"ci_cmd rc={rc}", actor=args.actor))
            return 1
    if cmd_done(reg, state, args) == 0:
        print(f"[完成] {args.id}")
        return 0
    cmd_block(reg, state, argparse.Namespace(id=args.id, reason="gate-failed", actor=args.actor))
    return 1


def main():
    ap = argparse.ArgumentParser(prog="taskctl")
    ap.add_argument("--actor", default=os.environ.get("GITHUB_ACTOR", "local"))
    sub = ap.add_subparsers(dest="cmd", required=True)

    p = sub.add_parser("plan"); p.add_argument("--check", action="store_true")
    sub.add_parser("status")
    p = sub.add_parser("next"); p.add_argument("--json", action="store_true")
    for name in ("start", "gate", "done", "reopen", "run"):
        p = sub.add_parser(name); p.add_argument("id")
    p = sub.add_parser("block"); p.add_argument("id"); p.add_argument("--reason", required=True)

    args = ap.parse_args()
    reg, state = load_registry(), load_state()
    handler = {
        "plan": cmd_plan, "status": cmd_status, "next": cmd_next, "start": cmd_start,
        "gate": cmd_gate, "done": cmd_done, "block": cmd_block, "reopen": cmd_reopen, "run": cmd_run,
    }[args.cmd]
    return handler(reg, state, args)


if __name__ == "__main__":
    sys.exit(main())
