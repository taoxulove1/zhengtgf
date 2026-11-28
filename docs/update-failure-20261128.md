# 登录器「游戏更新失败」根因与修复（2026-11-28 08:59）

## 1. 现象

登录器点「开始游戏」→ 进度条走到「当前文件 100% / 全部文件 100%」→ 弹窗：

```
游戏更新失败，请确认您的机器是否属于以下可能导致失败的原因：
1、网络错误导致不能从服务器上下载文件
2、游戏所在的磁盘空间已满导致不能写入文件
3、游戏文件夹目录是只读的导致不能写入文件
```

磁盘：E: 134G 可用，排除原因 2。目录 ACL 正常，排除原因 3（详见第 5 节）。真实原因是 1。

## 2. 证据链

服务器 `/root/zt/www/http.log`（客户端 120.228.118.238，08:59）：

```
08:59:09  GET /autopatch/srvlist.xml.co     404  → 回落 srvlist.xml 200
08:59:09  GET /autopatch/board.htm          200
08:59:11  GET /autopatch/flist.xml.co       200
08:59:16  GET /autopatch/data/datas.pak.co  404  ← 失败点 1
08:59:20  GET /autopatch/err.txt.co         404  ← 失败点 2
```

本地 `源1/patchupdate.log` 03:54 段同样记录了 `/autopatch/err.txt.co 404`。

## 3. 根因

### 3.1 datas.pak 条目错位（人为）

04:25 执行 `tools/push_flist.py` 时，本地 `源1/flist.xml` 里 `data/datas.pak` 条目是实验期残留：

| 项 | 值 |
|---|---|
| 服务器 flist 记录 | size=**3135868** crc=**2f505fc6** |
| 客户端本地实际 | size=**3135764** crc=**adcc4d81** |
| 服务器 `data/datas.pak` | **不存在**（从未上传） |

size 差 104 字节 → 客户端判定需更新 → 请求 `.co` → 404。

### 3.2 err.txt 被纳入 flist 校验（设计缺陷）

`err.txt` 是登录器**每次启动追加一行时间戳**的日志：

```
03:34, November 28, 2026
03:36, November 28, 2026
...
03:56, November 28, 2026
```

每行 21 字节。flist 记录 size=104 crc=d22f99ce，本地实测 130 字节。**只要启动一次就必然不匹配**，然后请求 `err.txt.co` → 服务器无此文件 → 404。

`tools/regen_flist.py` 的排除表里有 `*.log/*.bak/*.tmp/*.dl`，但没有 `err.txt`，所以被扫进清单。

## 4. 修复

| # | 动作 | 落点 |
|---|---|---|
| 1 | `flist.xml` 的 `data/datas.pak` 条目恢复为 3135764 / adcc4d81 | 服务器 autopatch、autopatchhuaijiu、Zhengtu/Client/autopatchhuaijiu 三路径（.xml + .co） |
| 2 | 删除 `err.txt` 条目（435 条 → 现 435 条，原 436） | 同上 |
| 3 | `源2/Data/items.ghd`(69668) 复制到 `源1/Data/items.ghd` | 本地，crc=d439c10d 与 flist 一致 |
| 4 | 上传 `datas.pak`(3135764) + `.co`(2541023, raw deflate) 到 `data/` | autopatch、autopatchhuaijiu，双保险 |
| 5 | `regen_flist.py` 排除表加入 `err.txt` / `*.pristine` / `*.orig*` / `*.noLAA.bak` | 防复发 |

`flist.xml` 现 54764 字节 / 435 条 / .co 10157 字节 / inflate 逐字节一致。

## 5. 排除项（已实测）

- **磁盘**：E: 341G，可用 134G。
- **只读属性**：`Data\datas.pak`、`patchupdate.exe` 均为 `A`（无 R）。
- **目录 ACL**：`E:\源端\源1` 与 `config.ini` 均为 `Authenticated Users:(M)`，可写。
- **`E:\源端\官方\config.ini`**：存在继承的 `WIN-6H6V930271M\Administrator:(I)(DENY)(W,D)`。该目录全树 mtime 停在 11-27 09:26，且 `patchsetup.ini` 为 534 字节（flist 记录 496），08:59 无任何写入痕迹 → 确认运行目录不是它。此 DENY 需要管理员权限才能解除，暂留。

## 6. 验证

对客户端 435 条清单做 size + CRC 全量校验（`crc = ~zlib.crc32`，引擎算法为 zlib 多项式 init=0xFFFFFFFF 无 final xor）：

```
entries 435   elapsed 3.8s
MISSING   : 0
SIZE bad  : 0
CRC  bad  : 0
READ error: 0
```

服务端 `flist.xml` 的 datas.pak 条目：

```
<file size="3135764" path="data/datas.pak" crc="adcc4d81" must="1" .../>
```

## 7. 遗留

- 注册表 `HKLM\SOFTWARE\WOW6432Node\上海征途网络科技有限公司\征途\InstallPath` = `E:\源端\源1\datazhengtu.dav`（把文件名拼进了安装目录），建议改为 `E:\源端\源1`。改动需管理员。
- flist 中 17 个根文件（d3dx9_37.dll、hprotect.exe、patchupdate.exe 等）服务器只有 `patchsetup.ini.co` / `patchupdate.exe.co` 两个 `.co`。当前客户端全部匹配故不触发下载；一旦本地被改就会 404。根治办法是全量上传或把 flist 收窄到只保留必须分发项。
