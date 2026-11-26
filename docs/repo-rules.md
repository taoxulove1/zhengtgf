# 仓库规则 · zhengtugf

> 生效范围：`main` 分支 + 全部 `release/*`。变更本文件需 2 名 CODEOWNER 审批。

---

## 1. 分支模型

| 分支 | 用途 | 生命周期 | 直推 |
|---|---|---|---|
| `main` | 唯一发布线，始终可构建 | 永久 | 关闭（含管理员） |
| `feat/<phase>-<slug>` | 功能开发，如 `feat/p2-protocol-doc` | ≤ 14 天 | 允许（本人分支） |
| `fix/<slug>` | 缺陷修复 | ≤ 7 天 | 允许 |
| `data/<slug>` | 资产清单 / 数据更新 | ≤ 7 天 | 允许 |
| `release/v<major>.<minor>.<patch>` | 冻结验证 | 发布后删除 | 关闭 |

命名正则：`^(feat|fix|data|chore|docs)/[a-z0-9][a-z0-9-]{2,40}$`，分支名不合规的 PR 由 `01-validate` 直接失败。

---

## 2. 合并策略（`main`）

| 项 | 设定 | 理由 |
|---|---|---|
| 合并方式 | **Squash only**（关闭 merge commit / rebase merge） | 线性历史，回溯单点 |
| 线性历史 | 强制（`required_linear_history: true`） | `git bisect` 可用 |
| 审批数 | ≥ 1，且必须含路径 CODEOWNER | 专业口把关 |
| 新提交失效旧审批 | 开启（`dismiss_stale_reviews: true`） | 防"过审后加料" |
| 会话解决 | 必须（`required_conversation_resolution: true`） | 遗留 comment 不放行 |
| 状态检查 | `validate` / `task-gate` 必过 | 二值门禁 |
| 强制推送 / 删除 | 均关闭 | 保护基线 |
| 管理员豁免 | 关闭（`enforce_admins: true`） | 规则对团长同样生效 |
| 合并队列 | 开启（如仓库套餐支持） | 避免串行回归 |

下发命令：`bash tools/protect-branch.sh`（读 `GITHUB_TOKEN` + `REPO`，幂等，可重复执行）。

### 2.1 免费私有库下的等效保护（当前生效方案）

GitHub 免费私有库不开放 branch protection 与 rulesets，API 返回
`Upgrade to GitHub Pro or make this repository public to enable this feature`。
在升级 Pro 或转 public 之前，用**两道本地/CI 闸**等价覆盖四条硬约束：

| 约束 | 闸一：本地 `pre-push` | 闸二：`05 主分支守门`（push 后） |
|---|---|---|
| R1 提交信息规范 | 逐条正则校验，不合规拒绝推送 | 违反则回滚 + 开 `[GUARD]` Issue |
| R2 线性历史 | 分支名白名单 | 检出 merge commit 即回滚 |
| R3 单文件 ≤ 100 MB | — | `ls-tree -l` 扫描，超限回滚 |
| R4 禁历史重写 | — | `merge-base --is-ancestor` 判定，非祖先即回滚 |
| 审批 / 会话解决 | 约定 + PR 模板清单（无强制力） | 无替代 |

```bash
pwsh -File tools/install-hooks.ps1     # 启用本地钩子（core.hooksPath=.githooks）
python tools/guard_main.py --before <sha> --after <sha>   # 本地复跑同一判据
```

**诚实边界**：review 强制、`enforce_admins`、合并队列这三项无免费替代。升级路径见 §2.2。

### 2.2 解锁正式保护的两种方式

| 方式 | 动作 | 代价 | 可逆 |
|---|---|---|---|
| 转 public | `PATCH /repos {"private":false}` 或 Settings → Danger Zone → Change visibility | 服务端源码、582 条资产清单、协议文档全部公开 | 可，但 fork 与缓存已扩散 |
| 升 Pro | 个人 \$4/月，或 Team \$4/人/月 | 私有库获得完整保护 + rulesets | 可，降级后规则失效 |

升级后重跑 `bash tools/protect-branch.sh`，回读确认 `enforce_admins=true`，
再按 §5 把 `github-actions[bot]` 加入 bypass，否则 `03 执行` 无法回写 `state/queue.json`。

---

## 3. 提交信息规范（Conventional Commits + 中文正文）

格式：

```
<type>(<scope>): <subject 中文，≤ 50 字>

<body 中文，说明动机与影响，每行 ≤ 72 字>
<空行>
<footer：Refs #12 / BREAKING CHANGE: ...>
```

`type` 白名单（其余 `01-validate` 拒绝）：

| type | 用途 | 示例 |
|---|---|---|
| `feat` | 新能力 | `feat(proto): 补齐背包 12 条消息定义` |
| `fix` | 修复 | `fix(server): 修正 SessionServer 长度前缀越界` |
| `data` | 资产 / 数据 | `data(manifest): 刷新 81 个 pak 的 sha256` |
| `docs` | 文档 | `docs(deploy): 补 systemd 自愈验证步骤` |
| `chore` | 构建 / 依赖 | `chore(ci): 上调解压超时到 15 分钟` |
| `refactor` | 重构（不改行为） | `refactor(net): 抽出 ByteBuf 读写指针` |
| `perf` | 性能 | `perf(session): 心跳扫描改时间轮` |
| `test` | 测试 | `test(qa): 增补 32 条用例矩阵` |
| `revert` | 回滚 | `revert: feat(proto) ...` |

`scope` 白名单：`server` `client` `proto` `db` `deploy` `qa` `ci` `docs` `manifest` `config` `tools`。

硬性约束：
- subject 不以句号结尾，不用 `update`/`修改`/`优化` 这类无信息词。
- 单次提交改动 ≤ 800 行（资产清单与自动生成文件豁免）。
- 一个 PR 只做一件事；跨阶段改动必须拆分。

---

## 4. 权限与审核矩阵

| 角色 | GitHub 权限 | 可推分支 | 可批 PR | 可动 `main` 保护 | 典型人员 |
|---|---|---|---|---|---|
| Admin（仓库所有者） | Admin | 全部 | 是 | 是 | 仓库 owner |
| Maintainer | Maintain | `main` 经 PR | 是 | 否 | 陆全通 |
| Developer | Write | `feat/*` `fix/*` `data/*` | 是（非本人 PR） | 否 | 文定协 / 柯正栈 / 盛久稳 / 褚安库 / 毕联通 |
| Reporter | Read + Issue | 无 | 否 | 否 | 外部协作 |

路径级 CODEOWNERS（见 `.github/CODEOWNERS`）——命中路径的 PR 必须由对应 owner 批：

| 路径 | Owner | 覆盖阶段 |
|---|---|---|
| `/src/server/` `/config/server/` | 柯正栈 | P3 / P4B |
| `/src/client/` `/src/client-legacy/` `/config/client/` | 文定协 | P1 / P2 |
| `/docs/protocol/` | 文定协 | P2 |
| `/assets/manifest/` | 褚安库 | P4A |
| `/docs/deploy/` | 盛久稳 | P4B |
| `/.github/workflows/` `/tools/` `/tasks/` | 陆全通 | 全局 |
| `/docs/qa/` | 毕联通 | P5 |

禁止事项：本人 PR 自批；用 `admin` 绕过保护（`enforce_admins` 已开）；在 `main` 上直接 `git push --force`。

---

## 5. 文件结构与数据更新约定

### 5.1 落位规则

| 类型 | 落位 | 命名 | 禁止 |
|---|---|---|---|
| 服务端源码 | `src/server/<进程名>/` | 保持原目录名 | 改进程名 |
| 客户端源码 | `src/client/<模块>/` | 保持原目录名 | 把 `boost_1_34_1` 入库 |
| 客户端文本配置 | `assets/client/<原相对路径>` | 原文件名 | 改编码（统一 UTF-8 或原 GBK 并登记） |
| 大资源 | `assets/paks/<原相对路径>`（不入库） | 原文件名 | 入库、改名 |
| 协议文档 | `docs/protocol/protocol.md` + `msgid.csv` | 固定名 | 另起文件名 |
| 任务定义 | `tasks/registry.json` | 固定名 | 手改 `state/` |

### 5.2 数据更新五步（缺一步 CI 失败）

1. 改源数据（客户端目录 / 服务端配置）。
2. `python tools/gen-manifest.py --source <源目录> --out assets/manifest` 重算 sha256。
3. `python tools/verify-assets.py --root <源目录>` 全量比对，0 差异。
4. 提交：`data(manifest): <变更摘要>`，PR 打 `data:change` 标签，需 2 人复核（1 名 CODEOWNER + 1 名其他成员）。
5. 大资源变更同步发 Release：`04-assets-release` 自动切 90 MB 分卷并上传。

### 5.3 版本与标签

- `VERSION` 文件 + `CHANGELOG.md` 同步改，PR 内完成。
- 标签格式 `v<major>.<minor>.<patch>`，仅 Maintainer 可打。
- 阶段门禁通过时打预发布标签：`v0.<phase>.0-rc.<n>`。

---

## 6. 授权与合规边界

- 本仓库仅服务**已获授权的本地测试环境**：客户端、服务端、数据库均在自控主机内。
- 客户端二进制改动（patch / 反校验）一律先整包冷备，改动记录写入 `docs/patches/`（含文件 SHA256、偏移、原字节、新字节）。
- 仓库内不存放真实账号、支付凭据、玩家个人数据；测试账号统一 `test_*` 前缀。
- 第三方代码（boost / lua / zlib / luabind / greta）保留原许可声明，不改协议头。
