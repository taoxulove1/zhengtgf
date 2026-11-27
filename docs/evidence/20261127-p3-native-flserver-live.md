# P3 收尾：原生 FLServer 已在真实端口跑通，客户端接入面从 0 字节变为正常应答

> 一句话：原版 `FLServer_r.exe` 的客户端接入面（8000/8002）在 Wine 下**不可能**产生有效回包——
> 不是采集方法问题，也不是缺一台带管理员权限的隔离 Windows。用原生 FLServer 接管同一组端口后，
> 真实官方客户端发出的原始字节拿到了 1328 字节合法应答（解帧后是 zlib 压缩的 `0x010B`）。

## 1. 先定死「原版接入面不可用」

在**全新干净重启**的原版全栈上（`tools/stack_restart.sh`，MySQL 4.0 起来、7 个服务全部「启动服务成功」）复测：

| 目标 | 空连 | 发 0x0101 校验帧（RC5 加密） | 发 0x1E ping hello+request |
|---|---|---|---|
| 原版 8000 | 挂住不关 | 挂住不关、无回包 | 收到 request 后 `FIN` |
| 原版 8002 | **立即 FIN** | 立即 FIN | `BrokenPipeError(32)` |
| 原版 8001 | 挂住不关 | — | — |

`BrokenPipeError` 与 Codex 日志里的 `[FAIL][ORIGINAL_BYTES] [Errno 32] Broken pipe` **完全同因**，
说明这不是它那台机器的问题，是原版二进制在该环境下的固有行为。

同一时刻的旁证：

- `ss -tn state established` 里 **8001 有一条真实 ESTABLISHED**（`10.1.0.10:53062 <-> 10.1.0.10:8001`），
  FL 日志有 `zTCPTask::getNextState(10.1.0.10:42696), verify -> sync` / `ServerTask::verifyConn`；
- 而 8000/8002 上从来没有一条 `LoginTask`/`PingTask` 的日志。
- `IocpMonitor.log` 里 7 个服务的 `Iocp 完成端口启动` 全部正常，没有 accept 报错。

结论：**只有服务间（8001）的接入在 Wine 下可用，客户端接入面（8000/8002）不可用。**
这就是为什么官方客户端连上去只发不收（`captures/cap-hs.pcapng`：客户端发了 16B，10 秒后重发，服务端 0 回复）。

## 2. 真实客户端首包的明文（顺带解出来了）

`captures/cap-hs.pcapng` 里有一条真实外网客户端（`120.228.118.238`）打 8000 的记录，两个 16 字节包：

```
包1  ac60efdf6546a7d3b6bab7d27ce5e098      RC5-32/12/16 解密后 ->
     0c 00 00 80 | 01 01 | 80 da 08 6b | cf 07 00 00 | 00 00
     └ len=12, ENC=1       └ 0x0101   └ ts          └ version=1999  └ 填充
包2  0a00000001018ada086bcf0700000000      明文同构，len=10、ENC=0，10 秒后重发（降级重试）
```

要点（写进协议文档）：

- 首包 opcode **0x0101**，body 固定 10 字节：`byCmd=01, byParam=01, dwTimestamp, dwVersion=1999`；
- 加密帧的长度头 **12**（含 2 字节零填充），明文帧长度头 **10**（填充不计入长度）——两种语义并存，解析要兼容；
- `0c000080` 说明 **ENC 标志打在长度头 bit31**，且长度头本身参与加密，与 `docs/protocol/protocol.md §0.1` 一致；
- 客户端在无响应 10 秒后**自动把加密降级为明文重发**，这是一条此前没记录的行为。

## 3. PING 口协议（0x1E，三步）

`PingTask::verifyConn()` 要求**首包必须是 ping hello**，否则 `verifyConn` 返回 -1、连接被回收：

```
hello    06 00 00 00 | 1e 03 00 00 00 00                        (10 B, byParam=3)
request  0a 00 00 00 | 1e 02 00 00 00 00 | 01 00 01 00          (14 B, 末尾 4B 是 zoneId)
reply    1b 00 00 00 | 1e 01 00 00 00 00 | 01 00 01 00 | 16×00 | 00   (31 B)
```

原生回包实测：

```
1b000000 1e01 00000000 01000100 0000000000000000000000000000000 00
```

即 `<BBI>` = (cmd=30, param=1, ts=0) + zoneId(4) + 16 字节 0 + 1 字节状态。
`captures/native-8002-ping.pcap` 是这次交互的抓包。

## 4. 根因：原生构建缺 `<threadPoolCapacity>`，任务池容量为 0

直接用原生 `FLServer` + 仓库里的 `config.xml`（端口 8000/8001/8002）起，**三个端口连上即 FIN**，日志：

```
FLServer TRACE: PingTask::verifyConn
FLServer DEBUG: PingTask::verifyConn(30, 3)      <- hello 校验其实通过了
FLServer FATAL: zTCPTaskPool????????????????????  <- zTCPTaskPool没有找到合适的线程来处理连接
FLServer DEBUG: resetState(127.0.0.1:43170), sync -> recycle)
```

对照 `src/server/base/zTCPTaskPool.cpp:1169`：`zTCPTaskPool没有找到合适的线程来处理连接` → 任务池按状态分的线程容量为 0。
最小配置（Codex 的 smoke）里显式写了 `<threadPoolCapacity>64</threadPoolCapacity>`，所以能跑；仓库的 `config.xml` 里**一个都没有**。

修法（已实测生效）：

```bash
python3 - <<'EOF'
p="/root/zt/server/config.xml"; d=open(p,"rb").read()
for s in (b"FLServer",b"RecordServer",b"BillServer",b"SessionServer",
          b"GatewayServer",b"MiniServer",b"SuperServer",b"ScenesServer"):
    d=d.replace(b"<"+s+b">", b"<"+s+b"><threadPoolCapacity>64</threadPoolCapacity>", 1)
open(p,"wb").write(d)
EOF
```

注入 8 段后，同一二进制、同一份真实配置（打补丁备份在 `config.xml.bak-threadpool`）：
8000/8001/8002 全部由原生进程监听，ping 返回 31 字节，登录口返回 1328 字节。
**原生 8 个服务都要补这一段**，这是 P4B 部署的硬前置。

## 5. 原生 8000 对真实客户端首包的应答

请求 = 真实客户端原始密文 `ac60efdf6546a7d3b6bab7d27ce5e098`（未做任何改造，原样重放）：

| 项 | 值 |
|---|---|
| 服务端回包长度 | **1328 B** |
| 帧头（RC5 解密后） | `dword0 = 0x4000052C` → len=1324，**ZIP=1**，ENC=0 |
| body 前两字节 | `78 9c` = zlib 流 |
| 解压后 | 1477 B，起始 `01 0b 00 00 00 00` → opcode **0x010B**，随后 `ff d8 ff e0 … JFIF` 即 JPEG |

换成我们自己合成的 0x0101（ts 不同）→ 回包 1144 B，解压 1292 B，同样 `0x010B` + JPEG，仅图片长度不同（1469 vs 1029）。
说明：0x0101 校验通过后，FL 下发 **0x010B（压缩）**，里面带一张 JPEG（`<process_file>process.dat` / 特征码图路径相关），长度随时间/文件内容变化，**不能作为逐字节固定基准**。

产物已入库：

- `captures/native-8000-reply-to-real-client.bin`（1328 B）
- `captures/native-8000-reply-to-synthetic.bin`（1144 B）
- `captures/native-8000-login-verify.pcap`
- `captures/native-8002-ping.pcap`

## 6. 下一步（已定位，不是猜）

**6020 起不来的真因**已在 Gateway 日志里写死：

```
创建到服务器(10.1.0.10:7020)的连接成功
创建到服务器(10.1.0.10:7010)的连接成功
zSubNetService::getServerEntryByType(type=23)
服务器信息：23, 11 / 23, 12 / 23, 20
ERROR 不能找到小游戏服务器相关信息，不能连接小游戏服务器
ERROR 启动服务失败...
```

MiniServer 自己是 `ServerID=23, IP=10.1.0.10, Port=6012`，6012 也在听；但 Gateway 拿到的 type=23 列表里没有它认的那一条。
所以下一步是**核对 Super 下发的小游戏服务器条目**（`zSubNetService::getServerEntryByType(23)` 的匹配键），不是改端口、不是等时间。

顺序：

1. 对齐 MiniServer 的注册类型/ID 与 Gateway 的匹配条件 → 6020 起来；
2. 6020 起来后，用官方客户端点一次登录，服务器侧 `tcpdump` 存档 → 这才是 P3 的最终 golden（客户端自己认，比任何字节比对都硬）；
3. 再回头处理 ScenesServer（VMProtect，见 `20261127-scenesserver-blocked-by-vmprotect.md`），它不影响登录，只影响进图。

## 7. 补充实测（同日 20:49–21:07）

### 7.1 6020 网关的真因是「启动顺序」，不是端口

Mini 报到 Super 是 `20:31:34`，Gateway 去问 Super 是 `20:31:32` —— 差 2 秒。
`getServerEntryByType(23)` 遍历 `serverList` 只打印出 `11 / 12 / 20` 三种 type，没有 23（`MINISERVER`，见 `base/zMisc.h:43`），于是 `GatewayServer.cpp:158` 直接 `return false`。

把 `srv.sh` 的 `ORDER` 从

```
super record bill fl session gate scene mini
```

改成

```
super record bill mini fl session gate scene
```

按新顺序重启 Gateway 后：`创建到服务器(10.1.0.10:6012)的连接成功` → `启动服务成功...` → 6020 开始监听。
**MiniServer 必须先于 GatewayServer 向 Super 注册**，这条要写进 P4B 的 runbook。

### 7.2 原生与 Wine 原版混跑，服务间协议不兼容

原生 `ScenesServer`（`build/p3-native/ScenesServer`，8.06 MB ELF32）在补齐 `threadPoolCapacity` 后能起线程池、能连上 Super：

```
zTCPTaskPool::init
... 线程池容量 64, 最大连接 512 ...
zTCPClient::connect
INFO: 创建到服务器(10.1.0.10:10000)的连接成功
ERROR: (zSubNetService::init)等待服务器管理器返回信息失败
```

`sendCmd(t_Startup_Request)` 发出去了，回包收不到 —— 原版 Super 直接关连接，且 Super 侧日志此后 8 分钟无任何写入。
判断：原生 GCC 编译的 `t_Startup_Request` 与原版 MSVC 的字段布局/对齐不一致（原生带 `-D_USE_CMD_NAMESPACE -D_USE_EPOLL_`）。

**结论：不能混跑。要么全原生 8 个服务，要么全原版。**
现状「原生 FL + 原版其余」是刻意为之——原版 FL 的客户端接入面在 Wine 下不通（§1），其余服务用原版是为了保住已验证可用的内部互联。
下一步是整栈换原生（`build/p3-native/` 下 8 个产物齐全），用同一套编译产物保证协议一致。

### 7.3 帧解码器的两个坑（已在工具里处理）

- **ENC 位在长度头里，长度头本身又被加密**：必须先解 8 字节才知道长度，但不知道长度又不知道解多少。
  解法：同时计算「解出来的头」和「原始头」，用长度合法性（1..1 MiB）+ ENC 位判断该信谁。
- **加密帧长度含零填充、明文帧不含**：真实客户端首包加密时 `len=12`（10 字节 body + 2 填充），
  降级明文重发时 `len=10`（填充不进长度）。切帧一律按 `align8(4+len)` 前进，body 只取 `4..4+len`。

## 8. 整栈换原生的实测结论（21:20–22:00）

### 8.1 挡路的第一层：Super 的依赖表是空的

现象：Super/Record/Bill/Mini/FL 全起来了，`Session`、`Gateway`、`Scenes` 一律报
`getServerEntryByType(11)` 找不到条目（Session 报「不能找到档案服务器相关信息」），随后 segfault 退出。

源码定位（`SuperServer/ServerTask.cpp`）：

```cpp
static void initServerSequence() __attribute__ ((constructor));   // 211 行
void initServerSequence() {
    serverSequence[RECORDSERVER] = std::vector<int>();
    int data0[] = { RECORDSERVER };
    serverSequence[SESSIONSERVER] = std::vector<int>(data0, data0 + 1);   // Session 依赖 Record
    ...
}
```

`serverSequence` 是同一个 TU 里的 `static __gnu_cxx::hash_map`。GCC 下 `__attribute__((constructor))`
的执行时机与普通静态对象的构造顺序**没有保证**，实测就是先跑了 `initServerSequence()`、
后构造 `hash_map` —— 写进去的表被后续构造清空，于是 `processSequence()` 遍历到空 vector。

日志正是这个形状：

```
SuperServer TRACE: ServerTask::processSequence(wdServerType=20)
（此后没有任何 verifyTypeOK(wdType=11) 的 TRACE，说明循环体一次都没进）
```

修法（已验证）：去掉 `__attribute__ ((constructor))`，在 `processSequence()` 里显式初始化一次：

```cpp
bool ServerTask::processSequence()
{
    static bool __seq_inited = false;
    if (!__seq_inited) { initServerSequence(); __seq_inited = true; }
    if (hasprocessSequence) return true;
    ...
```

重编 SuperServer 后：Session 越过依赖检查、开始连 Record(7010)；Gateway 和 Scenes 也不再秒退（进程能活着）。

### 8.2 挡路的第二层：原生源码版本比部署包新

Session 越过依赖后卡在两处，都是**数据/配置与新代码不匹配**：

```
SessionServer ERROR: Unknown column 'BOSSEXP' in 'field list'
SessionServer ERROR: SELECT SEPTID, NAME, CHARID, MASTER, `VOTE`, `CREATETIME`, `REPUTE`, ...
I/O warning : failed to load external entity "Config/GameConfig.xml"
SessionServer ERROR: ...读取配置失败 Config/GameConfig.xml
```

- 部署的 Zebra 库里没有 `BOSSEXP` 列（06 版 schema），
- 整台机器上也**没有** `Config/GameConfig.xml`（`src/server/SessionServer/GameConfigMgrX.cpp`、
  `ScenesServer/GameConfigMgr.cpp` 都要它），源码树里也不带这份数据文件和建表脚本。

也就是说 `p3-934bf0d` 这棵源码树对应的服务端版本 ≠ 部署包（06 版）的 DB/配置。
继续走全原生，就得先把 DB schema 补到新版本 + 造出 `Config/GameConfig.xml`，成本不可估。

### 8.3 结论与现行方案

| 方案 | 结果 |
|---|---|
| 全原生 8 服务 | Super 的坑已修（可复用），但卡在 schema + 配置缺失；6010 仍起不来 |
| **原生 FL + Wine 其余（现用）** | 9/10 端口在听，只有 6010 缺；客户端登录面正常（8000 有合法回包） |

现行部署（已恢复并核对）：

```
8000/8001/8002  原生 FLServer
10000 Super / 7010 Record / 7020 Bill / 6012 Mini / 6001 Session / 6020 Gateway   Wine 原版
6010 Scenes     缺（Wine 版被 VMProtect 拦，原生版卡 schema）
```

### 8.4 config.xml 的三个 patch 与副作用（重要）

排查过程中对 `/root/zt/server/config.xml` 做过三处改动，副作用不一样，回滚时别搞混：

| patch | 内容 | 影响 | 现在状态 |
|---|---|---|---|
| `threadPoolCapacity` | 8 个服务段各插 `<threadPoolCapacity>64</threadPoolCapacity>` | 原生构建必需（不补则任务池容量 0，连接一律 FATAL）；对 Wine 原版无害 | **保留** |
| `logfilename` | `log/xxx` → `log/nat/xxx.log` | 原生 log4cxx 要文件；但 **Wine 原版要求它是目录**，改成文件后点「启动服务」会静默失败（进程在、窗口在、端口不起） | **已回滚** |
| `ifname` | `10.1.0.10` → `eth0` | 原生 `zSocket::getIPByIfName` 用 ioctl SIOCGIFADDR 要网卡名；Wine 原版沿用 IP。改错会导致 Super 绑定失败（`DEBUG: 0.0.0.0` + 绑定错误） | **已回滚为 IP** |

备份文件：`config.xml.bak-threadpool`、`config.xml.bak-logfile`、`config.xml.bak-ifname`。

### 8.5 原生构建侧的两个坑

- 链接：`preflight` 探针要 `-llua -lluabind`，系统里没有 32 位版本；静态库在
  `/root/p3-test/p3-lua-deps/lib`（`liblua.a` / `libluabind.a`），
  用 `export LIBRARY_PATH=/root/p3-test/p3-lua-deps/lib:/usr/local/lib` 即可，不必改脚本。
- 单服务重编：`build_native_server.py` 用 `Path(__file__).parents[1]` 定位 ROOT，
  把裁剪版脚本放到 `/root/` 会算出 `ROOT=/` 并报 `Missing source Makefile: base/EncDec`；
  必须放在仓库 `tools/` 下（`tools/build_super_only.py`）。另外输出目录 `build/p3-native`
  已存在时会直接拒绝，需要先 `mv` 开。

工具：`tools/native_stack.sh`（整栈原生 + 两个已知坑的处置）、`tools/fix_super_build.sh`（单编 SuperServer）。
