## 变更说明

<!-- 一句话说清「改了什么 / 为什么」。对应任务：P0/P1/P2/P3/P4A/P4B/P5/P6 -->

- 阶段：
- 关联任务单：#

## 改动类型

- [ ] feat 新能力
- [ ] fix 缺陷修复
- [ ] data 资产 / 数据更新
- [ ] docs 文档
- [ ] chore 构建 / CI
- [ ] refactor / perf / test

## 自查清单（缺项会被 01-validate 拦）

- [ ] 分支名符合 `^(feat|fix|data|chore|docs)/[a-z0-9-]{3,40}$`
- [ ] 提交信息符合 Conventional Commits（`type(scope): 中文简述`）
- [ ] `python tools/validate_repo.py --ci` 本地通过
- [ ] 无单文件 > 100 MB；大资源已走 Release 分卷
- [ ] 改了数据 → 已重跑 `gen-manifest.py` 并提交 `assets/manifest/*`
- [ ] 改了协议 / 配置 → 同步更新 `docs/protocol/` 或 `config/`
- [ ] 涉及客户端 patch → 已写 `docs/patches/<sha256>.md` 并备份原文件
- [ ] 未提交真实账号、支付凭据、玩家个人数据

## 验收证据

<!-- 贴关键输出：make 退出码、ss -lntp、pcap 文件名、sha256 比对结果 -->

```
（粘贴）
```

## 影响面

- 影响进程：
- 影响端口：
- 回滚方式：
