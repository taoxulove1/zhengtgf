# 贡献指南

规则全文在 [`docs/repo-rules.md`](docs/repo-rules.md)，此处只列最短路径。

## 三条命令自查

```bash
python tools/validate_repo.py --ci        # 体积 / JSON / 分支 / 提交 / 密钥
python tools/validate_repo.py --manifest  # 资产清单自洽
python tools/taskctl.py status            # 我在哪个阶段
```

## 五条硬规则

1. 分支：`feat|fix|data|chore|docs/<slug>`，直推 `main` 关闭。
2. 提交：`type(scope): 中文简述`，type 白名单见规范 §3。
3. 合并：Squash only、线性历史、1 名 CODEOWNER 批准、会话全解决。
4. 体积：单文件 ≤ 100 MB；大资源切 90 MB 分卷挂 Release。
5. 数据：改了就重跑 `gen-manifest.py`，PR 打 `data:change`，2 人复核。

## 任务怎么走

```bash
python tools/taskctl.py next      # 下一个可执行任务
python tools/taskctl.py start P1  # 置 running（前置未完成会被拒）
python tools/taskctl.py gate  P1  # 跑门禁
python tools/taskctl.py done  P1  # 门禁过才置 done
```

Issue 打 `task:run` 标签 = 在 CI 里走同一条链路。
