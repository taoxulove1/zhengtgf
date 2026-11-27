# 改动记录 · patchsetup 本地化

| 项 | 值 |
|---|---|
| 目标文件 | `patchsetup.ini`（客户端根目录） |
| 原文件 SHA256 | 见 `assets/manifest/assets.csv` 中 `patchsetup.ini` 行 |
| 原文件存档 | `assets/client/patchsetup.ini`（未改动，原样入库） |
| 替换文件 | `config/client/patchsetup.local.ini` |
| 改动时间 | 2026-11-27 |
| 改动原因 | 将页面、账号入口和错误上报改为本地配置；旧启动器曾实际触发在线更新，见 [客户端基线](../client-baseline.md) |
| 备份 | `E:/源端/backup/zhengtugf-client-3.1.46.0.7z`（MD5 `a8116110…`） |

## 逐键对照

| 键 | 原值 | 新值 |
|---|---|---|
| `logo` | `http://www.ztgame.com.cn/p2p/flash.shtml` | `about:blank` |
| `board` | `http://zthj.ztgame.com/p2p/board.htm` | `about:blank` |
| `home` | `http://zthj.ztgame.com` | `about:blank` |
| `pay` | `http://pay.ztgame.com` | `about:blank` |
| `account` | `https://account.ztgame.com.cn:8080/` | `http://127.0.0.1:8080/` |
| `register` | `http://counter.ztgame.com/ref.php?ref=82087&...` | `about:blank` |
| `BugAddr0` | `218.80.198.252` | `127.0.0.1` |
| `BugPort0` | `9902` | `9902`（不变） |
| `BugAddr1` | `210.22.188.20` | `127.0.0.1` |
| `BugPort1` | `9902` | `9902`（不变） |
| `bugobj` | `/bugs/bugreport.php` | 不变 |
| `protectedAccountAddr` | `http://zthj.ztgame.com/kszy/tsxt/ccbh.shtml` | `about:blank` |

## 回滚

```
copy assets\client\patchsetup.ini <客户端目录>\patchsetup.ini
```

2026-11-27，在测试副本 `E:/源端/源` 中将根目录的 `patchupdate.exe` 改名为 `patchupdate.exe.bak`。备份的 SHA-256 为 `f8d635187cc69538a540421273eca381114250ee657cbb4ba604461568c80613`，与改名前一致。需要恢复测试副本的更新器时，将备份改回原名。

用户提供的属性截图确认桌面「征途怀旧版」快捷方式指向 `E:/源端/源/patchupdate.exe`，起始目录为 `E:/源端/源`。更新器程序中还有官服补丁列表地址。改名后这个快捷方式的原目标已不存在。测试副本的启动行为和测试服登录入口仍待验证。

## 官方登录器复核

用户选择原安装包的官方登录器。当前测试副本中的 `patchupdate.exe.bak` 与 `E:/源端/original/patchupdate.exe` 的 SHA-256 一致。隔离目录中另有曾在线更新过的版本，原安装包副本用作本次基线。

新解压目录 `E:/源端/源1` 与原始基线 `E:/源端/original` 均包含 579 个文件。递归比对相对路径、大小和 SHA-256，差异为 0。`imversion.ini` 记录 `3.1.46.0`，`patchupdate.exe` 的哈希与上文备份一致。分析使用工作区中的单文件副本。

静态核对确认，程序读取 `[Setup]` 的 `updateSelf`，对应标志用于跳过启动器自身的 `patchupdate.exe` 更新项。区服列表获取还有独立调用，使用内置的 `srvlistall.ztgame.com.cn`，并设有数字 IP 回退。测试副本 `patchsetup.ini` 的页面地址本地化后，这个区服列表调用仍存在。`config.ini` 里有测试服登录地址和端口，但官方列表加载后还会按所选区服把 `loginaddress`、`loginport` 和 `zone` 写入该文件。

对新解压版本复查时，在 PE 默认映像基址 `0x00400000` 下，`0x00402DFE` 触发区表获取，`0x00416950` 根据内置服务器条目调用下载函数，`0x00416780` 通过 `DownLoadF.dll` 读取数据，下载完成后进入列表解析。`0x00416BED` 起的代码将选中区服的地址、端口和区号写入根目录 `config.ini`。本次检查未找到从客户端根目录加载 `srvlist.xml` 的入口，原始目录也未附带本地区表文件。单独给根目录增加一个 `srvlist.xml` 尚无验证依据。

测试副本里的官方更新器仍保留为 `.bak`。复用官方入口需要验证它能加载测试区服，且启动和刷新区服时不会触发官服补丁下载。P3 只需在测试区服列表取得原版 `8002` 的正常响应。

## 复验

1. 启动客户端，观察从双击到登录框的耗时（期望 ≤ 3 秒，改前 6–13 秒）。
2. 抓包确认无对 `ztgame.com` 的 DNS 请求。
3. `imversion.ini` 未被本次改动触碰（版本号仍为 `3.1.46.0`）。
