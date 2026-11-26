# captures

抓包落盘目录。文件不入库（体积与隐私），只入库 `index.csv` 摘要。

## 为什么本机暂时抓不到

| 检查 | 结果 |
|---|---|
| `pktmon` 存在 | 是（`C:\Windows\system32\PktMon.exe`） |
| 当前会话管理员 | **否**（`IsInRole(Administrator)` = False） |
| `tshark` | 未安装 |

`pktmon start --capture` 与 `pktmon etl2pcap` 都需要管理员权限，因此 `captures/*.pcapng` 目前为空，
P1 门禁的第三项（≥1 个 pcapng）无法满足。

## 一条命令解锁

```powershell
# 右键「以管理员身份运行」PowerShell，然后：
cd E:\源端\zhengtugf
pwsh -File tools\capture.ps1 -Seconds 20
```

脚本会：`pktmon start` → 启动 `E:\源端\官方\ztconfig.exe` → 等 20 秒 → `pktmon stop` →
`pktmon etl2pcap` 转出 `captures/boot.pcapng`，并写 `index.csv`（文件名、字节数、sha256、时间）。

## 抓完怎么用

```bash
python tools/decrypt.py --pcap captures/boot.pcapng --port 7001 --key 133457799BBCDFF1
```

`--pcap` 走 tshark 取 `tcp.payload`；若本机无 tshark，用 Wireshark 导出 "Raw" 后：

```bash
python tools/decrypt.py --file exported.bin --key 133457799BBCDFF1
```

## 验收要点

1. 改过 `patchsetup.local.ini` 后，抓包中**不应出现**对 `*.ztgame.com` 的 DNS 查询。
2. 客户端首个 TCP 连接的目标 `IP:PORT` 即登录服地址，记入 `docs/protocol/protocol.md §0`。
3. 首包解密后应能以 `0x0101 USERVERIFYVER` 开头（18 字节），这是握手第 1 步。
