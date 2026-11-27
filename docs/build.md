# P3 服务端原生构建

当前已接入仓库的原生构建入口。Windows 本机预检确认缺少 Linux 环境。仓库配置的远端构建机连接在当前会话中也被拦截。真实构建、端口监听和通信验收待完成。

## 构建

从仓库根目录在 Linux 上执行。

```bash
make -C src/server-rebuild preflight PYTHON=python3
make -C src/server-rebuild PYTHON=python3
python3 tools/validate_repo.py --phase P3
```

预检会用 `g++ -m32` 实际编译、链接一个小程序。各开发库需与 x86 ABI 匹配。原 Makefile 使用 MySQL 兼容客户端、libxml2、log4cxx、GD、JPEG、OpenSSL、zlib、Lua 5.1 和 luabind。查询工具应支持 `mysql_config --libs_r` 及 `xml2-config`。预检通过后，构建入口在临时源码副本中调用各模块原有 Makefile，按库和服务的依赖顺序工作。

构建入口兼容目标机记录的 Python 3.6。它将预检使用的 `gcc` 和 `g++` 名称传给子 Make，确保实际编译使用同一套 PATH 工具链。目标机编译器及开发库的当前状态仍需连通后核验。

成功后 `build/p3-native/` 应包含 SuperServer、FLServer、RecordServer、BillServer、MiniServer、SessionServer、ScenesServer 和 GatewayServer 八个 x86 Linux ELF。构建失败时以首个编译或链接错误为准。现有输出目录会被保留，复跑使用新的检出目录或先移走已验收的本地产物。

## 运行验收

文件门禁只检查 ELF 产物。P3 仍需按任务登记验证构建退出码、测试端口 `7001` 监听、正常报文的收发字节与样本一致，以及连续收发 100 条正常报文后进程仍在运行。测试使用隔离配置，并依据 [P2 协议正文](protocol/protocol.md) 选择消息。当前 Windows/Wine 样本的端口与原生验收配置可能不同，分别记录实测值。

P3 完成后，队列才会解锁 P4A 数据库联调和 P4B 部署。P5 等两项通过后开始。
