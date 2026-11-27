# P3 服务端原生构建

2026-11-27，CentOS 7 实机已通过 32 位编译与 ELF 产物门禁。现场日志显示 `Built 8 services`、`[P3_BUILD_READY]` 和 `P3 script exit: 0`。紧接着复跑时，构建入口发现已有产物目录并退出。运行验收使用第一轮产物。

## 构建

从仓库根目录在 Linux 上执行。

```bash
make -C src/server-rebuild preflight PYTHON=python3
make -C src/server-rebuild PYTHON=python3
python3 tools/validate_repo.py --phase P3
```

预检会用 `g++ -m32` 实际编译、链接一个小程序。各开发库需与 x86 ABI 匹配。原 Makefile 使用 MySQL 兼容客户端、libxml2、log4cxx、GD、JPEG、OpenSSL、zlib、Lua 5.1 和 luabind。查询工具应支持 `mysql_config --libs_r` 及 `xml2-config`。预检通过后，构建入口在临时源码副本中调用各模块原有 Makefile，按库和服务的依赖顺序工作。

构建入口兼容目标机记录的 Python 3.6。它将预检使用的 `gcc` 和 `g++` 名称传给子 Make，确保实际编译使用同一套 PATH 工具链。

成功后 `build/p3-native/` 应包含 SuperServer、FLServer、RecordServer、BillServer、MiniServer、SessionServer、ScenesServer 和 GatewayServer 八个 x86 Linux ELF。构建失败时以首个编译或链接错误为准。现有输出目录会被保留，复跑使用新的检出目录或先移走已验收的本地产物。

## 运行验收

先把 `tools/p3_runtime_inventory.sh` 放到构建机的 `/root/p3-test/`，核对产物及运行库，收集测试端口、网络命名空间和本机数据库工具的状态。脚本同时识别首轮成功日志和复跑时写出的 `[WORKDIR]`。

```bash
cd /root/p3-test && bash p3_runtime_inventory.sh p3-build-luabind.log
```

运行库检查通过后，将 `tools/p3_runtime_smoke.py` 上传到同一目录。它用独立网络命名空间和临时 MariaDB 数据目录启动这批产物中的 `FLServer`。7001 保持服间端口，100 次正常 PING 请求走 7002。脚本核对前后监听进程身份，并保存首包原始字节与源码定义的对照。空网关列表的最后一个状态字节未由旧服务端初始化，因此只比较其余字节，仍需同场景的原版抓包完成独立对照。

```bash
cd /root/p3-test && python3 p3_runtime_smoke.py p3-build-luabind.log
```

2026-11-27 的隔离运行已确认原生 `FLServer` 监听 `7001`，`7002` 收发 `100/100`，前后均为 PID `13627`。首个响应的 30 个已定义字节与源码对照一致。仓库现有的原版抓包只包含服间心跳，原版 PING 对照仍待验证。

已收到实机导出的两个原生 PING 文件。请求和回应的区号均为 `0x00010001`，请求与源码样本一致，回应的 30 个已定义字节也一致，本次响应的最后一个状态字节为 `0x00`。收到的 `.py` 附件与仓库的烟测脚本哈希一致。实机 `p3-runtime-smoke.log` 仍待归档，现有抓包中也没有可比对的原版响应。

| 原生实机文件 | 大小 | SHA-256 |
| --- | ---: | --- |
| `ping-request.bin` | 14 B | `a0adc50a709ce27f539ea51e5b598aed7433f5125c641f4657883a0fbdf44677` |
| `ping-reply.bin` | 31 B | `37d702227caa7a93dcc3fad5e8402a86c9ec82886580f47f9dce4ac3acf17791` |

可先从已完成的实机测试导出原生基线，无需重新编译或启动服务。在 Linux 测试主机执行以下命令。成功后传回输出路径所指的压缩包，包里只包含烟测日志和两个 PING 二进制文件。

```bash
(
  set -e
  cd /root/p3-test
  p3_dir=$(sed -n 's/^\[RUNTIME_DIR\] //p' p3-runtime-smoke.log | tail -n 1)
  p3_dir=${p3_dir%$'\r'}
  grep -q '^\[OK\]\[EXCHANGES\] port=7002 requests=100 replies=100 ' p3-runtime-smoke.log
  case "$p3_dir" in /root/p3-test/p3-runtime-*) ;; *) exit 1 ;; esac
  test -s "$p3_dir/ping-request.bin" && test -s "$p3_dir/ping-reply.bin" || exit 1
  p3_out=$(mktemp /root/p3-test/p3-native-evidence.XXXXXXXX.tar.gz)
  tar -czf "$p3_out" -C /root/p3-test p3-runtime-smoke.log \
    -C "$p3_dir" ping-request.bin ping-reply.bin
  tar -tzf "$p3_out"
  sha256sum "$p3_out"
)
```

`tools/p3_runtime_reference.py` 已确认原版 Wine `FLServer_r.exe` 的 `8002` 监听，但使用原生格式发起查询时连接提前关闭，返回 `Broken pipe`。改从与原版部署配套的客户端正常打开或刷新区服列表，采集 `8002` 上的请求和响应。比对时先核对是否为同一种请求。

后续收到的 `p8002.pcap` 有 39 条完整记录和 14 字节未完成的末尾记录。可读部分有 6 条 TCP 连接，5 条可见服务端 FIN 或 RST，服务端应用层负载为 0。同期截图出现 Wine 中的 `accept()` 10057。这些证据只说明原版监听仍需排查，尚无可比响应。先核对原版进程的监听和日志，再用配套客户端重采区服列表流量。

复核本地样本时，`p8002.b64` 解码为同一份截断文件，完整记录中客户端发往 `8002` 的 TCP 负载共 62 字节。`cap_hs.pcapng` 的 28 条 TCP 记录均涉及 `8000`。`cap_real.pcapng` 与 `cap_pu.pcapng` 字节相同，均无 `8002` 记录。原始样本保留在本机。

原版 `flserver_8000/20261127/102848.log` 的末尾 40 行反复出现 `ServerTask::verifyConn`。本仓库对应源码将 `ServerTask` 用于服间端口，PING 端口由 `PingTask` 处理。应检查整份日志的 `PingTask`、`LoginTask` 和套接字错误记录，再与抓包时间对照。

```bash
f=/root/zt/server/log/flserver_8000/20261127/102848.log; iconv -f GB18030 -t UTF-8 -c "$f" | grep -Ei 'PingTask|LoginTask|accept\(|10057|socket.*(error|fail)|8002' | grep -Eiv 'mysql://|passw(or)?d|token|secret|口令|密码' | tail -n 35
```

筛选截图中的最后一条 `LoginTask` 记录为 `13:54:35`，所查文件更新时间为 `14:07:48 +0800`。`p8002.pcap` 的可读 TCP 记录为 `16:01:46.352` 至 `16:01:52.863 +0800`，8002 在这段记录内收到了 62 字节应用层负载，响应应用层负载为 0。先用下列只读命令核对当前监听归属、进程启动时间和最近的原版日志，确认是否已有新的日志文件。

```bash
date '+%F %T %z'; ss -lntp 'sport = :8002'; ps -eo pid,lstart,stat,comm | grep -i '[f]lserver'; find /root/zt/server/log -type f -ipath '*flserver*' -iname '*.log' -printf '%TY-%Tm-%Td %TH:%TM:%TS %s %p\n' 2>/dev/null | sort -r | head -n 5
```

16:26 的复核显示 `8002` 仍归 `FLServer_r.exe` PID `17507`，最近的应用日志仍停在 14:07:48。原版启动脚本将 Wine 的标准输出和错误输出写入 `/root/zt/log/fl.out`。如果需要恢复原版测试服务，先查看这个文件和当前连接。在可以短暂停服的测试窗口，只用原版 FLServer 界面停止并重新启动该服务，再确认 `8000`、`8001`、`8002` 和日志更新时间。整栈启动脚本会停掉同一 Wine 前缀中的其他服务，不用于本次单服恢复。

```bash
ss -nt state established '( sport = :8000 or sport = :8001 or sport = :8002 )'; stat -c 'Wine output: %y | %s bytes' /root/zt/log/fl.out && LC_ALL=C grep -aEi 'accept\(|10057|unhandled|exception|bind|listen|backtrace' /root/zt/log/fl.out | grep -aEiv 'mysql://|passw(or)?d|token|secret' | tail -n 20
```

实机回传显示 `fl.out` 为 0 字节，只有一条 `8001` 服间连接，`8000` 和 `8002` 当时没有已建立的连接。单独重启原版 FLServer 时，这条服间连接也会短暂断开。恢复后在测试客户端刷新一次区服列表，无需提交账号。

下一轮的 45 秒监听结束时抓到 0 包。测试副本 `E:/源端/源` 的桌面快捷方式原来指向已停用的更新器，`Data/zhengtu.dat` 是客户端主体。另有一份登录器生成器，其自带列表未配置当前测试服务器，运行兼容性也未验证。下次采集前先确认使用的登录器和区服列表都指向测试服务。

Windows 客户端启动诊断源码位于 `tools/start-test-client.cs`。用 .NET Framework C# 编译器的 `/target:winexe /codepage:65001` 生成可执行文件，将它放到包含 `Data` 的客户端根目录。它读取根目录 `config.ini` 的 `[server]` 段，使用 `loginaddress`、`loginport` 和 `zone`，按仓库客户端源码的参数顺序启动 `Data/zhengtu.dat`。登录端口为 `8000`，区号为 `1`。本机以测试程序验证了 `.dat` 可执行文件、参数和工作目录。现有客户端用这组参数启动后立即以 `0x00000000` 退出，未显示登录窗口。备份更新器的启动格式与仓库客户端源码存在差异，现有二进制与该源码的启动兼容性待确认。

这个入口只负责启动游戏客户端，启动器本身不访问网络，也不调用 `patchupdate.exe`。当前本机的 `patchupdate.exe.bak` 是原更新器备份。独立启动入口没有区服列表页面，不能用它代替原版 `8002` 区服测速采集。

用户在平时使用的 Windows 会话中复测，诊断入口同样报告 `0x00000000`，未显示登录界面。测试副本的 `Data/zhengtu.dat`、`gui.pak`、`datas.pak` 和 `login.pak` 与原始基线的 SHA-256 一致。原更新器仍停用，静态检查显示它保留内置的官服区表请求。现有证据确认退出复现，具体退出分支尚未定位。

已检查两套本地登录器资料。`登录器生成器.zip` 中的 `GameLogin.exe` 默认启动 `Client_Release.exe`，更新组件为同包的 `Updater.exe`，区表还有预设的外部地址。IDA94 工具包中的 2019 通用登录器附带 `ZtGame.dat` 文件版本 `1.0.0.1200`。当前安装包的 `zhengtu.dat` 文件版本为 `3.0.0.2606`，安装包版本为 `3.1.46.0`。两套资料均未建立与当前安装包的启动兼容性。

在独立源码副本中运行 `Client.sln` 的 `Release|x86` 编译。首次运行发现 MFC 项目配置、Boost 缺失和旧 C++ 标识符冲突。补入已有的 Boost 1.34.1 并在副本验证兼容性调整后，当前首个编译错误为 `engine/include/engine.h` 中缺少 `d3d8.h`。后续链接还需要工程列出的 x86 DirectX 库，包括 `d3d8.lib`、`D3dx8.lib` 和 `Dxerr8.lib`。目前没有可用的 Windows 客户端构建产物。

```bash
ss -lntp | grep -E ':(8000|8001|8002)[[:space:]]'; ss -lntp 'sport = :8002' | grep -i 'flserver_r\.exe' && p3_capture=$(mktemp /root/p3-test/p3-original-ping.XXXXXXXX.pcap) && printf 'Capture: %s\n' "$p3_capture" && timeout -s INT 45 /usr/sbin/tcpdump -i any -nn -s 0 -U -w - 'tcp port 8002' > "$p3_capture"
```

等待 `tcpdump` 打印统计并退出后，传回完整 `.pcap` 文件和前述原生基线压缩包。以下命令在本地仓库根目录执行。第一条检查抓包记录是否完整，并统计 `8002` 流量。第二条在同一 TCP 会话中对比实际请求和响应，只有响应逐字节相等才输出 `[OK][ORIGINAL_RAW_BYTES]`。

```bash
python tools/p3_compare_ping_capture.py --inspect p3-original-ping.pcap
python tools/p3_compare_ping_capture.py p3-original-ping.pcap ping-request.bin ping-reply.bin
```

P3 完成后，队列才会解锁 P4A 数据库联调和 P4B 部署。P5 等两项通过后开始。
