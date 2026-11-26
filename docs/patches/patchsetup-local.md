# 改动记录 · patchsetup 本地化

| 项 | 值 |
|---|---|
| 目标文件 | `patchsetup.ini`（客户端根目录） |
| 原文件 SHA256 | 见 `assets/manifest/assets.csv` 中 `patchsetup.ini` 行 |
| 原文件存档 | `assets/client/patchsetup.ini`（未改动，原样入库） |
| 替换文件 | `config/client/patchsetup.local.ini` |
| 改动时间 | 2026-11-27 |
| 改动原因 | 官方已停止更新，8 个外网入口全部失效；`BugAddr` 不可达导致启动阻塞 3–10 秒，`account` 失效导致注册入口报错 |
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

## 复验

1. 启动客户端，观察从双击到登录框的耗时（期望 ≤ 3 秒，改前 6–13 秒）。
2. 抓包确认无对 `ztgame.com` 的 DNS 请求。
3. `imversion.ini` 未被本次改动触碰（版本号仍为 `3.1.46.0`）。
