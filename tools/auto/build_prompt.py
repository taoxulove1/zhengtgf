#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""build_prompt.py — 按 tasks/registry.json 生成给 Codex 的单任务指令。

用法:
    python tools/auto/build_prompt.py P2 > .codex/run-prompt.md
    python tools/auto/build_prompt.py --next > .codex/run-prompt.md
"""

import json
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
REGISTRY = os.path.join(ROOT, "tasks", "registry.json")
STATE = os.path.join(ROOT, "state", "queue.json")

RULES = """## 工作约定
1. 先读 `README.md`、`docs/output-rules.md`、`docs/repo-rules.md`，遵守其中的产出格式与提交规范。
2. 只改本任务需要的文件；不重构无关代码，不动 `tasks/registry.json`、`state/queue.json`、`tools/` 既有脚本、`.github/`。
3. 资料优先从仓库现有文件（docs/、captures/、src/、assets/、config/）推导。确实缺数据时，写出可复现的采集脚本与说明，并在产出文档中明确标注「未实测」，禁止编造字节、哈希、抓包内容。
4. 产出必须能被门禁命令验证通过；门禁不通过就修自己的产出，反复到 rc=0 为止。
5. 不要执行 `git commit` / `git push`，提交由流水线完成。
6. 最后一条消息列出：改动了哪些文件、门禁命令最后一次的退出码。"""


def load_state():
    if not os.path.exists(STATE):
        return {"tasks": {}}
    with open(STATE, encoding="utf-8") as f:
        return json.load(f)


def main():
    with open(REGISTRY, encoding="utf-8") as f:
        reg = json.load(f)

    tid = None
    if len(sys.argv) > 1:
        tid = sys.argv[1]
    if tid in (None, "--next"):
        state = load_state()
        todo = lambda x: state.get("tasks", {}).get(x, {}).get("state", "pending")  # noqa: E731
        for t in reg["tasks"]:
            if todo(t["id"]) == "pending" and all(todo(d) == "done" for d in t.get("deps", [])):
                tid = t["id"]
                break
        if tid is None:
            sys.exit("[ERR] 无可执行任务")

    task = next((x for x in reg["tasks"] if x["id"] == tid), None)
    if task is None:
        sys.exit(f"[ERR] 未知任务 {tid}")

    out = []
    out.append(f"# 任务 {task['id']} · {task['name']}\n")
    out.append(f"你是 {task['owner']}（{task['agent']}）。请在当前仓库里把这个任务真正做完，产出落到仓库对应路径。\n")
    out.append("## 输入（仓库内已有资料）\n")
    for i in task.get("inputs", []):
        out.append(f"- {i}")
    out.append("\n## 必须产出的文件 / 目录\n")
    for o in task.get("outputs", []):
        out.append(f"- `{o}`")
    gate = task.get("gate", {})
    out.append(f"\n## 门禁（必须自己跑到退出码 0）\n{gate.get('desc', '')}\n")
    out.append(f"```bash\n{gate.get('cmd', '')}\n```\n")
    if task.get("risks"):
        out.append("## 已知风险\n" + "\n".join(f"- {r}" for r in task["risks"]) + "\n")
    out.append(RULES)
    print("\n".join(out))


if __name__ == "__main__":
    main()
