# 部署 Runbook · Linux

> 状态：**骨架**（P4B 未开始）。命令按 `start.sh` 实测顺序编排，路径与端口为假设值，落地时回填。
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
