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

`tools/p3_runtime_reference.py` 已确认原版 Wine `FLServer_r.exe` 的 `8002` 监听，但使用原生格式发起查询时连接提前关闭，返回 `Broken pipe`。改从与原版部署配套的客户端正常打开或刷新区服列表，采集 `8002` 上的请求和响应。比对时先核对是否为同一种请求。

```bash
cd /root/p3-test && p3_capture=$(mktemp ./p3-original-ping.XXXXXXXX.pcap) && printf 'Capture: %s\n' "$p3_capture" && timeout -s INT 45 /usr/sbin/tcpdump -i any -nn -s 0 -U -w - 'tcp port 8002' > "$p3_capture"
```

P3 完成后，队列才会解锁 P4A 数据库联调和 P4B 部署。P5 等两项通过后开始。
