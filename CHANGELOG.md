# CHANGELOG

遵循 [Keep a Changelog](https://keepachangelog.com/zh-CN/1.1.0/)，版本号语义化。

## [Unreleased]

## [0.1.0] - 2026-11-27

### Added
- 仓库骨架：`src/`(server / client / client-legacy)、`assets/`、`config/`、`docs/`、`tasks/`、`tools/`、`state/`
- 源码入库：服务端八进程 2473 文件；客户端 2022 版；客户端旧版对照
- 文本配置入库：169 个 xml / ini / txt / css（`assets/client/`，保留原相对路径）
- 资产清单：582 条，sha256 全覆盖，四层分流（git 442 / git-optional 70 / lfs-or-release 55 / release-split 15）
- 任务流：`tasks/registry.json` 八节点 DAG + `tools/taskctl.py` 引擎 + `state/queue.json` 状态
- CI 四条工作流：01 校验 / 02 下发 / 03 执行 / 04 资产发布
- 规则：分支保护脚本、CODEOWNERS、PR 模板、三类 Issue 表单
- 文档：链路台账、架构拓扑、协议规范、部署 runbook、库基线、32 条用例矩阵、交付清单

### Known Issues
- 客户端整包冷备未做，`CLIENT_BACKUP_MD5` 为空（P0 门禁未过）
- 81 个 `.pak` 未发布 Release 分卷
- Actions 回写 `state/queue.json` 需把 `github-actions[bot]` 加入 bypass
