# 部署 Runbook · Linux

> 状态 **已完成场景资源预检，待配齐资源和启动验证**。下文的部署路径与端口仍需按目标机配置复核。
> 门禁：本文 ≥ 40 行且 `deploy/systemd/` 至少 1 个 unit 文件。

## 0. 前置

| 项 | 值 | 确认 |
|---|---|---|
| 发行版 | CentOS 7.9 x86_64 | ☐ |
| glibc | 2.17 | ☐ |
| 32 位库 | `glibc.i686 libstdc++.i686` | ☐ |
| 磁盘 | ≥ 50 GB | ☐ |
| 内存 | ≥ 8 GB | ☐ |
| 内网 IP | 192.168.1.10（假设） | ☐ |

### 场景资源预检

从仓库根目录执行 `python tools/check_scene_assets.py /opt/game/server`。默认使用服务器目录中的 `scenesinfo.xml`、`tbl/` 和 `map/`。实际配置使用其他目录时，传入 `--tables` 和 `--maps`。检查按 Linux 文件名大小写匹配。退出码 0 表示本节的静态检查通过，1 表示资源需补齐或复核，2 表示检查程序读取输入或解析源码失败。

TBL 数量、名称和记录宽度取自 `ScenesServer` 启动时调用的 `loadAllBM()` 及对应打包结构。地图核对静态分配的 ID、MPS 与同名 XML、MPS 头和瓦片长度，以及 XML 的 `map` 根节点。MPS 版本以源码声明的 `MAP_VERSION` 为预检标准，旧版候选需另外验证兼容性。

| 检查范围 | 通过的 TBL | 通过的 MPS/XML 对 | 未解析地图 ID |
| --- | ---: | ---: | --- |
| 旧 XP 服务端散装资源 | 17 / 62 | 22 / 236 | `248` |
| 商业服务端归档候选 | 19 / 62 | 22 / 236 | `248` |
| 两套候选按通过结构检查的文件名合并计数 | 19 / 62 | 22 / 236 | `248` |

商业归档补出 `ExperienceBase.tbl` 和 `ZhuanExperienceBase.tbl` 的合规宽度候选。前者有 160 条、每条 16 字节，仍需核验等级覆盖及所配游戏版本。两套候选的 `NpcBase.tbl` 都是 7,404 字节一条，当前源码要求 7,440 字节。`SoulStoneBase.tbl` 分别为 604 与 624 字节。两套资源里的地图同名关系也不足以确认配套，仓库样本 `qingyuancun-main.mps` 为 v6、368×485，旧包同名文件为 v7、300×400。

对商业服务端目录内另外 35 个直接嵌套归档做了条目列表检查。没有找到额外的启动必需 TBL 文件名。当前配置的地图 ID `248` 有启动引用，缺少 `mapinfo` 定义，须从同版本部署数据确认其映射。当前候选仅供比对，服务端启动和入图仍待验证。

## 1. 依赖安装

```bash
yum install -y glibc.i686 libstdc++.i686 libaio numactl chrony logrotate
ldd --version | head -1
```

## 2. 目录与属主

```bash
mkdir -p /opt/game/server /opt/game/logs
useradd -r -s /sbin/nologin game 2>/dev/null || true
chown -R game:game /opt/game
```

## 3. 内核与句柄

```bash
cat >> /etc/security/limits.conf <<'EOF'
game soft nofile 65535
game hard nofile 65535
EOF
sysctl -w net.core.somaxconn=1024
sysctl -w net.ipv4.tcp_tw_reuse=1
```

## 4. 启停顺序（对齐 start.sh）

| 序 | 命令 | 等待 |
|---|---|---|
| 1 | `SuperServer/SuperServer &` | 5 s |
| 2 | `RecordServer/RecordServer &` | 5 s |
| 3 | `BillServer/BillServer &` | 5 s |
| 4 | `MiniServer/MiniServer &` | 5 s |
| 5 | `SessionServer/SessionServer &` | 10 s |
| 6 | `ScenesServer/ScenesServer &` | 10 s |
| 7 | `GatewayServer/GatewayServer &` | — |

累计最小窗口 35 s；systemd 化时用 `After=` 串联，不用 `sleep`。

## 5. 端口与防火墙

```bash
firewall-cmd --permanent --add-port=7001/tcp
firewall-cmd --permanent --add-port=7002/tcp
firewall-cmd --reload
ss -lntp | grep -E '7001|7002|10000|3306'
```

## 6. 守护与日志

<!-- TODO: systemd unit ×N、Restart=always、RestartSec=3、logrotate 50MB×7 -->

## 7. 验收

- [ ] `ss -lntp` 三端口在听
- [ ] `systemctl status` 全部 active
- [ ] `kill -9` 后 3 s 自愈
- [ ] 外网 nmap 仅 7001 open
