# 目标服务器实测环境（P4B 基线）

> 实测时间：2026-11-27 08:12–08:20 CST
> 采集方式：`tools/server_probe.py`（paramiko 只读探针）+ `tools/remote.py`
> 凭据存放：`.secrets/server.local.json`（**已 gitignore**，不入仓库）
> 本文所有参数均为**实测值**，非推断；推断项单独标「待验」。

## 1. 主机

| 项 | 实测值 |
|---|---|
| 公网 IP | `101.33.242.142`（`curl ifconfig.me` 与内网出口一致，无 NAT 转换层） |
| 内网 IP | `10.1.0.10`（配置里所有 server 列表都用这个地址） |
| 主机名 | `VM-0-10-centos` |
| OS | CentOS Linux 7 (Core)，`centos-release-7-6.1810.2.el7.centos.x86_64` |
| 内核 | `3.10.0-1160.119.1.el7.x86_64` |
| glibc | 2.17 |
| CPU | 4 vCPU，Intel Xeon Platinum 8255C @ 2.50 GHz |
| 内存 | 3693 MB，**swap = 0**（当前 used 583 MB / available 2818 MB） |
| 磁盘 | `/dev/vda1` ext4 40 GB，已用 6.2 GB（17%），单分区 |
| 时区 | Asia/Shanghai (+0800)，**NTP 未启用**（`NTP enabled: no`）→ 时间漂移会让 `dwTimestamp` 判据失真 |
| SELinux | Disabled |
| 防火墙 | firewalld inactive；iptables 三条链全 ACCEPT，仅挂 `YJ-GLOBAL-INBLOCK/OUTBLOCK` 两个 ipset（腾讯云镜） |
| SSH | OpenSSH 7.4，公钥指纹 `SSH-2.0-OpenSSH_7.4`，RTT ≈ 0.05 s |
| Python | `/usr/bin/python3` = 3.6.8，`python` = 2.7.5 |
| 编译器 | gcc/g++ 4.8.5，GNU Make 3.82 |
| 抓包 | **tcpdump 4.9.2 已装**（`/usr/sbin/tcpdump`），tshark **未装** |

## 2. 运行方式：Wine 跑 Windows 服务端

结论：**服务端不是 Linux 原生重编，而是 Windows PE 在 Wine 里跑**。这直接改写 P3/P4B 的技术路线。

| 项 | 实测值 |
|---|---|
| Wine | `/opt/wine32/bin/wine`，`wine-4.0.4` |
| WINEPREFIX | `/root/.wine32`，`WINEARCH=win32` |
| 显示 | Xvfb `:99`，监听 TCP `6099`（6000+99） |
| GUI 驱动 | `xdotool` 点击服务窗口工具栏「启动服务」按钮，坐标 `(18,33)`，随后 `Down` + `Return` 过弹窗 |
| 服务端根目录 | `/root/zt/server`（所有进程 cwd 均为此目录） |
| 数据库 | `/root/zt/mysql40`（Wine 版 MySQL 4.0），`mysqld.exe --standalone --console --basedir=Z:\root\zt\mysql40` |
| 日志 | `/root/zt/server/log/<服务名>_<端口>/<YYYYMMDD>/<HHMMSS>.log`，GB18030 编码 |

### 2.1 进程清单（实测）

```
SuperServer_r.exe    RecordServer_r.exe   BillServer_r.exe    FlServer_r.exe
SessionServer_r.exe  GatewayServer_r.exe  MiniServer_r.exe    mysqld.exe
```

顺序脚本 `/root/boot_all.sh` 实测为：
`Super → Record → Bill → FL → Session → Gateway → Scenes → Mini`，每服等窗口最长 180–300 s，起完校验端口。

> 与仓库早期 `docs/architecture.md` 的推断顺序（Super→Record→Bill→Mini→Session→Scenes→Gateway）不同：**Mini 实际最后起**，Scenes 在 Gateway 之后。已按实测为准，架构文档需同步。

## 3. 端口矩阵（`ss -lntp` 实测，2026-11-27 08:13）

| 端口 | 服务 | 归属进程 | 用途 |
|---|---|---|---|
| 22 | sshd | sshd | 运维 |
| 3307 | MySQL 4.0 | `mysqld.exe` | Zebra 库，Wine 内 |
| 6001 | SessionServer | wineserver | 会话 |
| 6010 | ScenesServer | **未监听（进程不在）** | 场景 |
| 6012 | MiniServer | wineserver | 小游戏 |
| 6020 | GatewayServer | **未监听** | 网关（客户端二段连接） |
| 7010 | RecordServer | wineserver | 存档 |
| 7020 | BillServer | wineserver | 计费 |
| 8000 | FlServer | wineserver | **客户端登录口（外网）** |
| 8001 | FlServer | wineserver | 服间内联 |
| 8002 | FlServer | `FlServer_r.exe` | 客户端 PING |
| 9967 | BillServer 列表 | 配置引用 | 计费端口（`billServerList.xml`） |
| 10000 | SuperServer | wineserver | 中央调度，`SuperServer.cpp: Port = 10000` |
| 6099 | Xvfb | Xvfb | X11 |

> `ss` 把 Wine 的 socket 归属报成 `wineserver`，所以"进程名对不上端口"是正常现象，**判活只认端口 + 进程同时存在**。

## 4. 关键配置（`/root/zt/server/config.xml`，GB2312）

| 键 | 实测值 | 影响 |
|---|---|---|
| `<mysql>` | `mysql://test:***@127.0.0.1:3307/Zebra` | 全服共用一个库 |
| `<ifname>` | `10.1.0.10` | 绑定内网地址，需网卡上真有该 IP |
| `<superserver port>` | `10000` | Super 端口 |
| `<login_port>` | `8000` | 客户端登录 |
| `<inside_port>` | `8001` | 服间 |
| `<ping_port>` | `8002` | 客户端测速 |
| `<jpeg_passport>` | `false` | **关掉了图形验证码** → 登录链路少一跳 |
| `<exp_rate>` / `<drop_rate>` | `10` / `10` | 经验/掉落倍率 |
| `<service_flag>` | `15` | 1邮件+2拍卖+4特征码+8送马 全开 |
| `<process_file>` | `process.dat`（83 B） | 登录成功后下发的特征码文件 |
| `<gm_mode>` | `10` | GM 模式开关 |
| `<writebacktimer>` | `600` | 档案回写 600 s |

`zoneInfo.xml`：`game=0 zone=1 ip=10.1.0.10 port=10000 name=征途`。
`loginServerList.xml`：FLServer `10.1.0.10:8001`，外层 `8000`。
`billServerList.xml`：`10.1.0.10:9967`，`sourceID=1`。
`zonelist.xml`：单区 `id=1 name=征途`。
`scenesinfo.xml`：4 个 scene server（id 21…），共 382 个 `<map>` 条目，`country` 8 个。

## 5. 数据库基线（`/root/zt/mysql40`，端口 3307）

| 项 | 实测值 |
|---|---|
| 版本 | MySQL 4.0 系列（`mysql.exe` 客户端自报 4.0；`information_schema` **不存在**，属 4.x 特性） |
| 可用账号 | `test`（配置内使用），`root` 口令与配置里 `root:2266` **不一致**，实测被拒 |
| 库 | `Zebra`、`mysql`、`test` |
| Zebra 表数 | **58 张**：`1ANSWER ACCPRIV ACT ALLY ALLYTIME ARMY AUCTION BALANCE CAPTAIN CARTOONPET CDAREGAMBLE CHANGENAME CHARBASE CHARTEST CITY CONSIGNGOLD CONSIGNGOLDHISTORY CONSIGNMONEY CONSIGNMONEYHISTORY COUNTRY DARERECORD DARE_CAPTION EMPERORFORBID FORBIDTALK FUND GAMETIME GIFT HEROLIST HOME KINGNAME LOGIN LOGINSTAT MAIL MINIGAME NPCDARE PKGAME RECOMMEND RECOMMENDSUB RESTORECARD RESTOREGOLD SAMPLERELATION SCHOOL SCHOOLMEMBER SEPT SEPTMEMBER SERVERLIST SN SORTLIST TANK TBL_BONUS TBL_PCARD TECH UNION UNIONMEMBER VOTE VOTEDPLAYER VOTEITEM ZONEINFO answer` |

> `1ANSWER` 与 `answer` 并存，说明建库脚本被执行过两次或大小写敏感性不同（Linux 下 MySQL 表名默认区分大小写）。建库基线要以 `sql/` 目录脚本为准复现，不要直接 `mysqldump` 克隆。

## 6. 当前故障与判据

| 现象 | 实测证据 | 判据 |
|---|---|---|
| ScenesServer 未运行 | `pgrep ScenesServer` 为空；`ScenesServer_r_exe_261127021755 .log` 停在 `WndBase.cpp(538) myWinMain` / `CWndBase::SetTitle` | Wine 下创建窗口标题失败 → 进程自杀 |
| Scenes 报错原文 | 弹窗 `System Error. Code: 5. Access denied.`（截图 `docs/deploy/img/scenes-error-code5.png`，X11 `import -window root` 抓取） | Code 5 = `ERROR_ACCESS_DENIED`。排除项：`10.1.0.10` 确实在 eth0 上（`inet 10.1.0.10/22`）、6010 端口空闲、`log/` 与 `map/` 均为 root 可写、`process.dat` 644 可读 → **不是网卡/端口/权限表观问题**，指向 Wine 文件系统映射（`Z:\征途\Code\...` 编译期路径）或某个独占句柄 |
| Gateway 未监听 6020 | 08:13 时 `ss -lnt` 无 6020；08:20 复核 **6020 已 LISTEN**（未重启进程） | 网关是**延迟开监听**，等 Scenes/内部注册到位才 bind。故"Gateway 挂了"是误判，真正缺的是 Scenes |
| 客户端进不去游戏 | Scenes 无进程 + 6010 无监听 | 修复顺序：先 Scenes 得 6010，再验 Gateway 6020 后能否进图 |

## 7. 复现命令

```bash
# 环境全量盘点（只读）
python tools/server_probe.py
python tools/server_probe.py --json          # 写 state/server-env.json

# 单命令
python tools/remote.py run "ss -lntp"

# 服务端抓包并取回（服务器侧 tcpdump，不需要 Windows 管理员权限）
python tools/remote.py capture 30 captures/win-login.pcapng "tcp and port 8000"

# 上传脚本
python tools/remote.py put tools/capture-server.sh /root/capture.sh
```

## 8. 待验清单

1. Gateway 真实监听端口（日志目录名 `gatewayserver_2200` 与配置 6020 冲突，需在 Scenes 起来后复测）。
2. `10.1.0.10` 是否绑定在 `eth0` 上（`ip addr` 未采集，配置里硬编码，若网卡只有公网 IP 则服间通信会走回环）。
3. Scenes 崩溃栈的完整回溯（需 `WINEDEBUG=+relay` 或 `winedbg`，代价高，优先级低于抓包）。
4. MySQL `root` 真实口令（配置里 `root:2266` 已失效，`ShowCharbase`/`LoadLogServer` 两个工具段依赖它）。
