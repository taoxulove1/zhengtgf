# 2026-11-27 16:00 会话截图存档与核实

原图：`20261127-p3-session-report.png`（78,638 B）

## 1. 截图原文要点

另一会话（用时 3 分 28 秒）声称：

| 项 | 声称内容 |
|---|---|
| 仓库 | `E:\源端\zhengtugf-work` |
| 分支 | `feat/p3-native-build` |
| 提交 | `657cd85 保存 P3 构建修复和验收记录`、`96c5922 记录测试客户端停用官方更新器的操作` |
| 状态 | 已提交到**本地**仓库，GitHub 尚未收到；该环境连不上 GitHub HTTPS 端口 |
| 待用户操作 | 在能访问 GitHub 的机器上执行 `git -C "E:\源端\zhengtugf-work" push https://github.com/taoxulove1/zhengtgf.git HEAD:refs/heads/...` |
| 后续 | 用 8002 的抓包文件比对 P3 原版报文 |

## 2. 本机核实结果

| 核实项 | 实测 |
|---|---|
| `E:\源端\zhengtugf-work` | **不存在** |
| 实际工作副本 | `E:\源端\源\zhengtugf-work`（多一层 `源`） |
| 另有目录 | `E:\源端\源\.zhengtugf-local\zhengtugf-P3-934bf0d` |
| 提交 `657cd85` | 存在，`fix(server): 完成 P3 原生构建并记录运行验收` |
| 提交 `96c5922` | 存在，`docs(client): 记录测试副本的更新器停用方式` |
| 该仓库 remote | 只有 `upstream → E:/源端/zhengtugf`（本地），**从未配置过 GitHub 远端**，所以"连不上 HTTPS"并非根因，是根本没指到 GitHub |

## 3. 已完成的补救（2026-11-27 16:02）

主仓库 `E:\源端\zhengtugf` 以 `work` 为本地远端接入该工作副本，并把四条分支推到 GitHub：

| 远端分支 | SHA | 说明 |
|---|---|---|
| `feat/p3-native-build` | `96c5922` | P3 原生构建 + 更新器停用记录（截图所指） |
| `docs/p2-protocol-workflow` | `e9b4491` | P2 核心协议验收 |
| `data/p4a-zebra-baseline` | `1bed6e6` | Zebra 旧库恢复验证 |
| `data/p4b-scene-assets` | `6d64dce` | 场景服启动资源校验 |

远端核对：`git ls-remote origin` 四条分支 SHA 与本地 `work/` 分支逐一相等。

## 4. 工作副本里尚未提交的产物

`E:\源端\源\zhengtugf-work` 工作区当时存在：

```
 M docs/build.md
?? captures/cap_hs.pcapng / .b64
?? captures/cap_pu.pcapng / .b64
?? captures/cap_real.pcapng / .b64
?? captures/now.png
?? tools/p3_compare_ping_capture.py
?? tools/p3_probe_8002.py
```

这些是 P3 验收的现场证据与探针脚本，本次**未**替其提交——属于另一会话的在途工作，需由其自行 commit 或另行授权后统一入库。

## 5. 对任务状态的影响

`feat/p3-native-build` 的内容落在 P3（C++ 服务端还原）与 P4B（部署）。但 `docs/architecture.md` 已实测确认目标机**跑的是 Wine 托管的预编译 PE**，服务端不需要 Linux 原生重编——P3 的"原生构建"在本项目里属于**备用路线**，不是主链路。主链路仍是：

```
E:\源端\官方(冻结取证) / D盘完整客户端  →  8000 登录  →  6020 网关  →  6010 场景  →  3307 MySQL
                                                                    ↑
                                                       当前唯一断点：ScenesServer 未监听
```
