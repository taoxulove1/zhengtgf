# captures

抓包落盘目录。大文件不入库（体积上限 + 隐私），小样本与 `index.csv` 摘要入库作为证据。当前入库样本仅 21 KB。

## 已到手的样本

| 文件 | 来源 | 内容 | 用途 |
|---|---|---|---|
| `server-internal-20s.pcapng` | 目标机 `101.33.**.**`，`tcpdump 4.9.2 -i any`，2026-11-27 08:14 | 5 条流 × 19 帧，`10.1.0.10:10000 → 内部端口`，opcode `0x0106`，14 字节/帧，1 帧/秒 | 实测证实帧层（4B 长度头）+ 内网不加密（bit31=0），见 `docs/protocol/protocol.md §0.5` |

解这个样本（**不需要 tshark**）：

```bash
python tools/pcap_lite.py captures/server-internal-20s.pcapng --list
python tools/pcap_lite.py captures/server-internal-20s.pcapng --stream 0 --hexdump 96
```

## 还缺的那一种：客户端登录流

P1 门禁要的是**客户端→登录服**的握手包（`0x0101` 版本校验 → `0x0102` 账号密码 → `0x0104` 下发 DES 密钥），样本里的内部心跳不能替代。

两条路，任选其一即可解锁：

### 路线 1：服务器侧抓（已通，最省事）

服务器上 `tcpdump` 现成，客户端连 `101.33.**.**:8000` 时抓 8000 口：

```bash
python tools/remote.py capture 30 captures/client-login.pcapng "tcp and port 8000"
```

### 路线 2：本机 Windows 侧抓（需管理员）

| 检查 | 结果 |
|---|---|
| `pktmon` 存在 | 是（`C:\Windows\system32\PktMon.exe`） |
| 当前会话管理员 | **否**（`IsInRole(Administrator)` = False） |
| `tshark` | 未安装 |

```powershell
# 右键「以管理员身份运行」PowerShell
cd E:\源端\zhengtugf
pwsh -File tools\capture.ps1 -Seconds 20
```

脚本会 `pktmon start` → 拉起 `E:\源端\官方\ztconfig.exe` → 20 秒 → `pktmon stop` → `etl2pcap`，
产出 `captures/boot.pcapng` 并写 `index.csv`。

## 抓到之后怎么解

```bash
# 1) 切流（纯 Python，Linux SLL / Ethernet / raw IP 都支持）
python tools/pcap_lite.py captures/client-login.pcapng --list
python tools/pcap_lite.py captures/client-login.pcapng --flow 8000 --stream 0 --out login.bin

# 2) 登录阶段：RC5-32/12/16，密钥硬编码（tools/decrypt.py 内置）
python tools/decrypt.py --file login.bin --algo rc5

# 3) 登录后阶段：DES-ECB，密钥取 0x0104 下发的 key[keyOffset:keyOffset+8]
python tools/decrypt.py --file after.bin --algo des --key <16位hex>
```

## 验收要点

1. 登录首包解密后应以 `0x0101 USERVERIFYVER` 开头；若解不出来，先用 `--no-frame` 排除帧层误判。
2. 首包长度头的 **bit31 必须为 1**（客户端链路置 ENC）；若为 0，说明该连接走了内部链路或抓错了端口。
3. `0x0104 stServerReturnLoginSuccessCmd` 里 `keyOffset`（偏移 75 附近）决定 DES 密钥起点，必须从该包现场取，不要硬编码。
4. 改过 `patchsetup.local.ini` 后，抓包中**不应出现**对 `*.ztgame.com` 的 DNS 查询。
