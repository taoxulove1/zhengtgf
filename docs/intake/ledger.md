# P0 · 链路台账 v1

> 生成时间：2026-11-27 02:45 CST · 负责人：陆全通 · 机器可读版：`config/variables.json`
> 门禁：15 项变量有值 + 客户端冷备 MD5 一致 + 清单 ≥ 580 条。

## 1. 变量表

| # | 变量 | 值 | 状态 | 来源 / 待办 |
|---|---|---|---|---|
| 1 | `CLIENT_DIR` | `E:/源端/original` | 已确认 | 原始安装包解压；`官方` 目录已被在线更新污染，见 `docs/client-versions.md` |
| 2 | `CLIENT_VERSION` | `3.1.46.0` | 已确认 | `imversion.ini` `[version]` |
| 3 | `CLIENT_FILE_COUNT` | 579 | 已确认 | `find . -type f \| wc -l` |
| 4 | `CLIENT_TOTAL_BYTES` | ≈ 6.1 GB | 约值 | 精确值见 `assets/manifest/assets.json` |
| 5 | `CLIENT_BACKUP_MD5` | `1028e2ba…4984`（树指纹）/ `a8116110`（实体包） | 已确认 | 冷备 6.1 GB + 聚合指纹，见 `assets/manifest/backup.md5` |
| 6 | `SERVER_SRC` | `/opt/game/server` | 假设 | 待盛久稳确认目标路径 |
| 7 | `SERVER_PKG` | `E:/源端/VS2022服务端server.rar`（31.7 MB） | 已确认 | 本地压缩包，已解至 `src/server/` |
| 8 | `SERVER_PROCESSES` | 8 个：Super / Record / Bill / Mini / Session / Scenes / Gateway / FL | 已确认 | `src/server/` 目录 |
| 9 | `DB_HOST` | `192.168.1.11` | 假设 | 待用户确认内网段 |
| 10 | `DB_PORT` | `3306` | 假设 | MySQL 默认 |
| 11 | `DB_VERSION` | `5.7.44` | 假设 | 待探测原库版本 |
| 12 | `GAME_PORT` | `7001` | 假设 | 待 P1 抓包实测 |
| 13 | `GATE_PORT` | `7002` | 假设 | 同上 |
| 14 | `GM_PORT` | `7003`（仅 127.0.0.1） | 假设 | 后台端口 |
| 15 | `INTRANET_CIDR` | `192.168.1.0/24` | 已确认 | 按默认段落地，变更只影响 P4B |
| 16 | `PUBLIC_EXPOSE` | `["7001"]` | 假设 | 单端口 NAT |
| 17 | `CCU_TARGET` | 200 | 假设 | P5 压测目标 |
| 18 | `DEPLOY_OS` | CentOS 7.9 x86_64 / glibc 2.17 | 假设 | 32 位兼容基线 |

**源码实测补充**：`src/server/SuperServer/SuperServer.cpp` 中 `Port = 10000`——超级服监听 10000，作为进程总控入口，部署时需一并放行（内网）。

## 1.1 客户端生命周期（2026-11-27 认定）

**官方已停止更新**，`3.1.46.0` 为终版。推论：版本校验值永久固定、协议抓一次即定版、
`patchsetup.ini` 8 个外网入口全部失效须本地化、可剥离 `dudu`/`epay`/`gdc` 共 171 MB。
详见 `docs/client-baseline.md`。

## 2. 已知资产

| 类别 | 路径 | 体积 | 说明 |
|---|---|---|---|
| 官方客户端 | `E:/源端/官方` | 6.1 GB / 579 文件 | 81 个 `.pak` 占 5852.9 MB |
| 服务端源码 | `src/server/` | 81 MB / 2473 文件 | 八进程 + Makefile + 启停脚本 |
| 客户端源码 2022 | `src/client/` | 362 MB | VS2022 工具链，boost 外置 |
| 客户端源码旧版 | `src/client-legacy/` | 296 MB | VC2005，仅对照 |
| 资产清单 | `assets/manifest/assets.json` | 582 条 / 6286.5 MB | sha256 全覆盖 |

## 3. 缺口

1. ~~客户端整包未做冷备~~（已完成，基线换为 `original`） → 已完成：实体包 6.1 GB（MD5 `a81161105e4a`）+ 树指纹 `1028e2ba…`。
2. 内网 IP 段、目标发行版未确认 → P4B 只能用默认值推进。
3. 原数据库未就位（无 dump、无 `.frm`）→ P4A 输入缺失。
4. 抓包环境未准备（无 Wireshark / dumpcap 安装记录）→ P1 首日要补。

## 4. 硬阻塞

| 项 | 卡住谁 | 解锁动作 |
|---|---|---|
| 客户端目录完整性（是否含全部 `.pak`） | P1 | 启动客户端跑一遍，确认无缺包报错 |
| 内网 IP 段与端口 | P4B | 用户给定 `192.168.x.0/24` 或实际段 |
