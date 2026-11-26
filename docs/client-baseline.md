# 客户端基线 · 版本冻结

> 事实认定（2026-11-27，用户确认）：`E:/源端/官方` 是**官方已停止更新**的老版本客户端，版本号 `3.1.46.0`，不再有新补丁。
> 这条事实改变了 P1–P5 的五个前提，逐条列出。

## 1. 版本冻结带来的五个确定性

| # | 原风险 | 冻结后 | 影响阶段 |
|---|---|---|---|
| 1 | 客户端自动更新导致 patch 失效，需每次重做 | 永久失效风险归零，patch 一次即长期有效 | P1 / P5 |
| 2 | 版本校验值随补丁变化 | `imversion.ini` 的 `3.1.46.0` 就是唯一真值，服务端可硬编码比对 | P3 / P5 |
| 3 | 补丁服下线导致启动卡在更新检查 | `patchsetup.ini` 全部 URL 已失效，**必须本地化**，否则客户端卡在登录前 | P1 |
| 4 | 协议随版本漂移，抓包结果过期 | 抓一次即可定版，`msgid.csv` 不再需要版本维度 | P1 / P2 |
| 5 | 新反作弊上线 | `HProtect.exe` 停留在 2011-11，属老式保护，静态分析成本低 | P1 |

## 1.1 反证：更新源仍存活（2026-11-27 07:56 实测）

上文"已停止更新"的判断被现场证据推翻：

| 证据 | 数值 |
|---|---|
| `patchupdate.exe` 进程 | 运行中，PID **23128**，经 `127.0.0.1:7897` 代理出网 |
| 正在下载的临时文件 | `cef/patchupdatehelp.exe.tmp`、`Data/map_4_pack/map_4/dengjifubun2.map.dl`、`Data/npc2af6.tmp` |
| 文件集变化（对入库基线） | 新增 **235**、缺失 **2**、体积变化 **62** |
| 总体积 | 6.1 GB → **6.4 GB**（579 → 812 文件） |
| `imversion.ini` | 仍为 `3.1.46.0`（补丁不改版本号） |

关键文件被替换：

| 文件 | 原 | 现 | 含义 |
|---|---|---|---|
| `Data/HPLoader.dll` | 172624 | **187256** | 保护组件升级 |
| `Data/HPLoader64.sys` | 172704 | **190840** | 内核驱动升级 |
| `Data/gui.pak` | 983500 | **1277484** | UI 资源扩容 |
| `Data/CheckMalicious.exe` | 723664 | 726936 | 检测逻辑更新 |

新增组件：`cef/libcef.dll`（49.8 MB）、`Data/browser/`（CEF 内嵌浏览器 + 20 个 locale）、`Data/3DVoiceEngine/zencdec.dll`。

**结论**：版本号冻结 ≠ 文件集冻结。存在一个可用的更新源，客户端每次运行都可能自我改写。

### 处置三步

1. 终止本次更新（保住当前文件集）：`taskkill /PID 23128 /F`
2. 阻断后续更新：把 `config/client/patchsetup.local.ini` 覆盖到客户端根目录的 `patchsetup.ini`
3. 断根：把客户端根目录的 `patchupdate.exe` 改名为 `patchupdate.exe.bak`

三条都做完、确认无 `.dl` / `.tmp` / `_temp` 残留后，再重跑 `gen-manifest.py` 定版——**不要在有下载进行中时冻结基线**。

## 2. 已失效的外部依赖（必须在架设前切断）

`patchsetup.ini` 原文 8 个外网入口，全部指向官方域名：

| 键 | 原值 | 现状 | 处置 |
|---|---|---|---|
| `logo` | `http://www.ztgame.com.cn/p2p/flash.shtml` | 404 | 置空 |
| `board` | `http://zthj.ztgame.com/p2p/board.htm` | 失效 | 置空 |
| `home` | `http://zthj.ztgame.com` | 失效 | 置空 |
| `pay` | `http://pay.ztgame.com` | 失效 | 置空 |
| `account` | `https://account.ztgame.com.cn:8080/` | 失效 | **改本地** `http://127.0.0.1:8080/` |
| `register` | `http://counter.ztgame.com/ref.php?...` | 失效 | 置空 |
| `BugAddr0/1` | `218.80.198.252:9902` / `210.22.188.20:9902` | 不可达 | 置空，避免连接超时拖慢启动 |
| `protectedAccountAddr` | `http://zthj.ztgame.com/kszy/tsxt/ccbh.shtml` | 失效 | 置空 |

落地文件：`config/client/patchsetup.local.ini`（不改原文件，改动记录见 `docs/patches/`）。
理由：客户端启动时按 `BugAddr` 做连通性探测，不可达会阻塞 3–10 秒；`account` 失效则注册/找回入口报网络错误。

## 3. 可剥离组件（老版本红利）

| 组件 | 体积 | 性质 | 处置 |
|---|---|---|---|
| `dudu/` | 141 MB | 嘟嘟语音 SDK（`DuDuUp.exe`、`avcodec-56.dll`、`HVideoRoom.exe`） | 可整块剥离，服务端无需支持 |
| `epay/` | 21 MB | 支付网关（`epay.exe`、`banks/`、`Register.exe`） | 剥离，架设环境无支付需求 |
| `gdc/` | 9.0 MB | `gdc.exe` 抓包/调试工具 | 剥离，仅逆向期本地保留 |
| `HProtect.exe` | 302 KB | 2011 年老式保护壳 | 逆向阶段分析对象，架设后剥离 |
| `HPLoader.dll/.sys/64.sys` | — | 内核态加载器 | 风险面最大的组件，架设环境不加载 |
| `Data/CheckMalicious.exe` | — | 恶意进程检查 | 剥离 |

剥离后客户端体积从 6.1 GB 降到约 **5.9 GB**（`Data/` 5.89 GB 保留），启动链路少 6 个进程。剥离清单见 `docs/patches/strip-list.md`（待 P1 落地时生成）。

## 4. 对逆向难度的影响（P1 重估）

| 项 | 老版本判断 | 依据 | 工时影响 |
|---|---|---|---|
| 加密强度 | 大概率 XOR / 简单 RC4 或 TEA | 2011–2019 年代端游常规、`EncDec` 模块独立存在 | 从 +5 人日降到 +1 人日 |
| 壳 | 老式保护，非 VMProtect/Themida | `HProtect.exe` 时间戳 2011-11，体积 302 KB | 静态脱壳可控 |
| 协议 | 长度前缀 + 明文 opcode，无 TLS | 同期引擎惯例 | `msgid.csv` 可从源码直接提取 |
| 源码可用性 | **客户端源码在手**（`src/client`） | 已入库 362 MB | 加密函数可读，不必全靠调试器 |

结论：P1 工时从 3–5 人日下调到 **2–3 人日**，且 `captures/*.pcapng` 的字段级校验可先由源码静态比对替代。

## 5. 版本校验的落地方式

```
客户端 imversion.ini  version = 3.1.46.0
        ↓ 登录第一步上传
服务端 config/server/login.ini  [version] expect = 3.1.46.0  check_enable = true
```

三处必须一致：`config/client/imversion.ini`、`config/server/login.ini`、`docs/architecture.md` 的版本表。改一处即视为破坏兼容，需 PR + 复核。

## 6. 新增的门禁约束（已加入校验）

- `CLIENT_EOL=true` 时，`config/client/imversion.ini` 的 `version` **不得变更**，变更即 CI 失败。
- 任何客户端二进制改动必须落 `docs/patches/<sha256前8位>.md`，含偏移、原字节、新字节。
- 剥离组件必须保留原文件备份（`E:/源端/backup/` 的 7z 冷备即为此用）。
