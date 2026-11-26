# zhengtugf · 征途客户端逆向架设仓库

> 官方客户端 `3.1.46.0`（`imversion.ini`）→ 抓包解密 → 协议文档 → C++ 服务端还原 → Linux 部署 → 联调验收。
> 全链路六阶段（P0–P6）串行推进，每阶段以**二值证据**过门禁：pcap / 退出码 / `ss -lntp` 输出 / SHA256。

---

## 1. 现状快照

| 项 | 数值 | 来源 |
|---|---|---|
| 客户端版本 | `3.1.46.0` | `assets/client/imversion.ini` |
| 客户端文件数 | 579 个 / 6.1 GB | `assets/manifest/assets.csv` |
| 资源包（`.pak`） | 81 个 / 5852.9 MB（占 95.6%） | 同上 |
| 文本配置（`.xml/.ini/.txt`） | 154 个 / 0.61 MB | `assets/client/` 已入库 |
| 服务端源码 | 81 MB，`BillServer/FLServer/GatewayServer/MiniServer/RecordServer/ScenesServer/SessionServer/SuperServer` 八进程 | `src/server/` |
| 客户端源码（2022 版） | 362 MB，VS2022 工具链 | `src/client/` |
| 客户端源码（旧版） | 296 MB，VC2005 工具链，作对照 | `src/client-legacy/` |
| 单文件上限 | 100 MB（GitHub 硬限，`01-validate` 会拦） | `.github/workflows/01-validate.yml` |
| 仓库体积目标 | ≤ 900 MB（不含 Release 附件） | `docs/assets-policy.md` |

---

## 2. 目录结构

```
zhengtugf/
├─ src/
│  ├─ server/            服务端源码 + 配置 + 启停脚本（八进程）
│  ├─ client/            客户端源码 2022 版（VS2022 / boost_1_34_1 外置）
│  └─ client-legacy/     客户端源码旧版（VC2005，仅作对照，不参与构建）
├─ assets/
│  ├─ client/            官方客户端**文本配置**（154 个，按原相对路径镜像）
│  ├─ paks/              大资源落盘位（.gitignore，由 Release 分卷还原）
│  └─ manifest/
│     ├─ assets.csv      579 行：path,size,sha256,group,route
│     ├─ assets.json     同上 JSON，供脚本消费
│     └─ checksums.sha256  sha256sum -c 可直接校验
├─ config/
│  ├─ client/            客户端配置基线（imversion / patchsetup / config）
│  ├─ server/            服务端配置模板（按 P4 实测值回填）
│  └─ schema/            JSON Schema（task / manifest）
├─ docs/
│  ├─ architecture.md    拓扑与进程关系
│  ├─ repo-rules.md      分支保护 / 提交规范 / 权限矩阵
│  ├─ assets-policy.md   数据分层与上传约定
│  ├─ protocol/          协议文档（P2 产出，九列表）
│  └─ deploy/            部署 runbook（P4B 产出）
├─ tasks/
│  ├─ registry.json      六阶段任务 DAG（唯一真源）
│  └─ schema.json        任务定义校验
├─ state/
│  └─ queue.json         运行时任务状态（由 Actions 回写）
├─ tools/
│  ├─ taskctl.py         任务流引擎：plan/next/start/gate/done/block
│  ├─ gen-manifest.py    资产清单生成
│  ├─ verify-assets.py   资产完整性校验
│  ├─ validate_repo.py   CI 仓库规则校验
│  ├─ fetch-assets.ps1   Release 分卷下载与还原
│  ├─ split-assets.ps1   90 MB 分卷切分
│  └─ protect-branch.sh  分支保护一键下发（REST）
└─ .github/
   ├─ workflows/         01 校验 / 02 下发 / 03 执行 / 04 资产发布
   ├─ ISSUE_TEMPLATE/    task / data-change / bug
   ├─ CODEOWNERS         路径级审核归属
   └─ pull_request_template.md
```

---

## 3. 资产四层策略

GitHub 硬限：单文件 100 MB、单次推送 2 GB、仓库建议 < 1 GB。6.1 GB 客户端**整体直推在物理上不可行**，因此按体积分层：

| 层 | 阈值 | 去向 | 数量 | 说明 |
|---|---|---|---|---|
| A 文本 | < 1 MB | Git 直接纳管 | 154 | xml / ini / txt / css，`assets/client/` |
| B 小二进制 | 1–10 MB | Git 直接纳管（可关） | 待统计 | 图标、字体、小 dll |
| C 中件 | 10–100 MB | Git LFS 或 Release 单卷 | 待统计 | 源码包、`.pdb`、大 dll |
| D 大件 | ≥ 100 MB | **Release 90 MB 分卷** | 81 个 pak 中多数 | 仓库只留 `assets.csv` 中的 sha256 |

完整清单见 `assets/manifest/assets.csv`，`route` 列即归属层。还原命令：

```bash
pwsh -File tools/fetch-assets.ps1 -Release v0.1.0 -Target assets/paks
python tools/verify-assets.py --root "E:/源端/官方" --manifest assets/manifest/assets.json
```

---

## 4. 快速开始

```bash
git clone https://github.com/taoxulove1/zhengtugf.git && cd zhengtugf

# 1) 看任务队列与下一个可执行任务
python tools/taskctl.py status
python tools/taskctl.py next

# 2) 本地校验仓库规则（CI 同款）
python tools/validate_repo.py

# 3) 重新生成资产清单（源目录有变动时）
python tools/gen-manifest.py --source "E:/源端/官方" --out assets/manifest
```

---

## 5. 任务流（自动下发 + 推进）

```
P0 收资 ──▶ P1 抓包解密 ──▶ P2 协议文档 ──▶ P3 服务端还原 ──┬─▶ P4A 数据库 ──┐
                                                          └─▶ P4B Linux ───┴─▶ P5 联调 ──▶ P6 交付
```

- **真源**：`tasks/registry.json`（DAG + 门禁命令 + 负责人 + 工时）
- **状态**：`state/queue.json`（`pending/running/blocked/done`）
- **触发**：`schedule`（每日 09:00 巡检）→ 自动建 Issue 下发；`issues: labeled task:run` → 执行；`workflow_dispatch` → 手动指定任务 ID
- **推进**：`03-execute` 跑完写回状态；前置未 `done` 则拒绝开跑并标 `blocked`
- **门禁**：`gate.cmd` 退出码 0 才算过，日志作为 Artifact 留档 30 天

详见 `docs/repo-rules.md` 第 5 节与 `.github/workflows/`。

---

## 6. 规范索引

| 主题 | 文件 |
|---|---|
| 分支保护 / 合并策略 | `docs/repo-rules.md` §1–§2 |
| 提交信息规范 | `docs/repo-rules.md` §3 |
| 权限与审核矩阵 | `docs/repo-rules.md` §4 |
| 文件结构与数据更新 | `docs/repo-rules.md` §5 / `docs/assets-policy.md` |
| 协议文档格式 | `docs/protocol/README.md` |
| 部署 runbook | `docs/deploy/runbook.md` |

---

## 7. 已知缺口（待补）

1. `assets/manifest/` 首次生成需源目录在位（`E:/源端/官方`）。
2. 81 个 `.pak` 需发布 Release 分卷后才能异地还原。
3. `config/server/` 为空模板，待 P4B 实测端口与路径回填。
4. `docs/protocol/protocol.md` 为空骨架，待 P1/P2 产出。
5. 分支保护需仓库管理员 Token 执行 `tools/protect-branch.sh`。

---

维护：陆全通（全链路架设总监） · 分支 `main` · 许可与授权见 `docs/repo-rules.md` §6
