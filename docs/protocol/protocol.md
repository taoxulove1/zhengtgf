# 协议文档 · 征途 3.1.46.0

> 状态：**骨架**（P2 未开始）。填写标准见 `README.md`。
> 门禁：本文件 ≥ 120 行且六类消息齐全，否则 `P2` 不得置 done。

## 0. 报文封装（P1 源码实测结论）

> **本轮更正两条早期误判**：① 有 4 字节长度前缀，不是"按 opcode 定长裸流"；② 有 zlib 压缩（`PACKET_FLAG_ZIP`），不是"无 zlib"。依据为 `src/client/Client/Client.cpp:61-63` 与同文件收发两段（收 155-232 / 发 372-460），逐行核对后重写本节。

### 0.1 帧格式（线上一字节不差）

```
+--------+--------+--------+--------+---------------------------+   +-- 8 字节对齐填充
| DWORD 长度头（4B，小端，本身也参与加密）| payload（length 字节）      |   | 补齐到 8 的倍数
+--------+--------+--------+--------+---------------------------+   +
  bit31 ENC  bit30 ZIP   bit23-0 length
```

| 字段 | 位宽 | 取值 | 依据 |
|---|---|---|---|
| `length` | 24 bit | `sendsize - 4`，即"加密区总长减长度头" | `Client.cpp:429` `(sendsize-sizeof(tPackLengthType))` |
| `PACKET_FLAG_ENC` | bit31 = `0x80000000` | 恒置位（`HAS_ENCRYPT` 已定义） | `Client.cpp:430` |
| `PACKET_FLAG_ZIP` | bit30 = `0x40000000` | `bufferSize > 32` 时置位 | `Client.cpp:381-383` |
| `PACKET_MAX_SIZE` | `0x00FFFFFF` | 长度提取掩码 | `Client.cpp:62` |
| `tPackLengthType` | `unsigned long` = 4 B（Win32） | 长度头类型 | `Client.cpp:61` |

关键推论：**长度头在第 0~3 字节，且被加密**。加密从 offset 0 开始按 8 字节切块，所以第 0 块 = 长度头 4 B + payload 前 4 B。收方必须先解密再读长度，`Client.cpp:115-142`（先解密 `m_recvBuffer`）→ `Client.cpp:165-169`（后读长度）顺序即此。

### 0.2 加密两阶段（源码实测，非推测）

| 阶段 | 算法 | 密钥 | 切换点 | 依据 |
|---|---|---|---|---|
| 登录前 | **RC5-32/12/16** | 16 B 硬编码 `1cc41924c27d56c4235cc2291ff126e0` | `m_bLoginRecv = true`（构造默认） | `D3DFont.cpp:849`（注释"客户端验证密钥"）<br>`Client.cpp:123-132` |
| 登录后 | **裸 DES-ECB** | 8 B，取服务端下发 `key[256]` 的 `keyOffset` 起 8 字节 | `0x0104` 收到后置 `m_bLoginRecv=false` / `m_bLoginSend=true` | `NetAutoConnect.cpp:146` `Client_SetKey(m_key,pKeyData,8)`<br>`Command.h:376-397` `stServerReturnLoginSuccessCmd` |

两算法共用同一套"分块"约定，因此可以互换而不用改外层：

```cpp
// CDes::encdec / EncDec 同构：每 8 字节一块，无 IV、无链接、无填充，尾部余数不处理
for (i = 0; i < size/8; ++i) crypt(&buf[i*8]);
return nLen - offset;          // 余数字节明文残留
```

| 层 | 实测值 | 依据 |
|---|---|---|
| 传输 | TCP，流式 | `m_recvBuffer` 按 `size & ~7` 切块，余数留到下次 |
| 半包处理 | `< 8` 字节直接 `continue`，等下一段 | `Client.cpp:115-118` |
| 压缩 | zlib `compress2/uncompress`，`RLE_ENCDEC` 已注释掉 | `Client.cpp:10` `//#define RLE_ENCDEC`；`Client.cpp:375-410`、`190-222` |
| 压缩阈值 | `bufferSize > 32` 才压，压缩不省也照发 | `Client.cpp:381` |
| 帧内消息 | `stNullUserCmd` 起步：`BYTE byCmd` + `BYTE byParam` + `DWORD dwTimestamp` | `Command.h:261` |
| opcode | `(byCmd << 8) | byParam`，形如 `0x0101` | 559 条已提取，见 `msgid.csv` |
| 尾部填充 | `align8(4 + len)` 多出的 0~7 字节被 `length` 计入，解析需容忍 | `Client.cpp:428`、`230` |

### 0.3 工作流（工具已实现，可直接跑）

```bash
# 自检（DES/RC5 标准向量 + 帧层往返 + 8 对齐）
python tools/decrypt.py --selftest

# 登录阶段抓包 → RC5 解 → 切帧 → 查 opcode 表
python tools/decrypt.py --pcap captures/login.pcapng --port 7001 --algo rc5

# 登录后阶段，密钥来自 0x0104 下发的 key[keyOffset:keyOffset+8]
python tools/decrypt.py --file captures/after_login.bin --algo des --key <16位hex>

# 对端不带长度头时跳过帧层
python tools/decrypt.py --file raw.bin --algo des --key <hex> --no-frame
```

实测样例（自造 RC5 帧，`0x0101` 版本校验）：

```
输入 24 字节 → 处理 24 字节，尾部余数 0 字节
明文前 64 字节: 14000080010144332211564552332e312e34362e30000000
                  └─ 0x80000014 = ENC | (24-4) ─┘
帧层：切出 1 帧，尾部残留 0 字节
  frame#0  len=20  flags=ENC  decompressed=20
  +0    0x0101  USERVERIFYVER  len=18  ts=287454020  564552332e312e34362e30 ("VER3.1.46.0")
```

### 0.4 服务端侧对称性待验

`src/server/` 下 `CDes.cpp:34-47` 只实现了 DES 路径，未见 RC5。两种可能：① 网关（`Gateway`）在登录前不加密、由客户端单方向 RC5；② 服务端 RC5 实现藏在 `SessionServer` 或网关的 DLL 里，源码树不全。**这一点必须在 P1 有真实抓包后判死**——判据是登录首个包的 bit31 是否为 1、以及 RC5 解密后 `0x0101` 的 `byCmd/byParam` 是否落在表内。在拿到 `captures/*.pcapng` 之前，本节 0.1/0.2 的客户端侧结论视为已定，服务端侧标"待验"。

### 0.5 实测抓包证据（2026-11-27，服务器侧 tcpdump）

抓包点在目标机 `101.33.**.**`（`tools/capture-server.sh`，tcpdump 4.9.2，`-i any`），取回后由 `tools/pcap_lite.py` 解链路层（Linux SLL）→ IPv4 → TCP → 流重组，**未用 tshark**。

样本：`captures/server-internal-20s.pcapng`，19 秒，5 条流，全部为 `10.1.0.10:10000 → 内部端口`（SuperServer 下发）。

| 观测项 | 实测值 | 意义 |
|---|---|---|
| 单流字节数 | 266 B | — |
| 帧数 | 19 帧 | 20 秒窗口内 1 帧/秒 |
| 单帧长度 | 14 B（4 头 + 10 体） | 与 §0.1 完全吻合 |
| 长度头 | `0a 00 00 00` = 10，**bit31=0** | 帧层存在且内网链路**不置 ENC** |
| body[0:2] | `01 06` | opcode `0x0106` |
| body[2:6] | `13 58 05 00` = `0x00055813`，下帧 `0x00055814` | **每秒 +1 的秒计数**，不是墙钟时间戳 |
| body[6:10] | `00 00 00 00` | 4 字节保留/参数 |
| 尾部残留 | 0 字节 | 帧长严格自洽，无填充 |

原始字节（流 #0 前 3 帧）：

```
0a 00 00 00 | 01 06 13 58 05 00 00 00 00 00
0a 00 00 00 | 01 06 14 58 05 00 00 00 00 00
0a 00 00 00 | 01 06 15 58 05 00 00 00 00 00
└─ length ─┘ └cmd┘└param┘└─ ts ─┘└─ 4B ─┘
```

结论与推论：

1. **§0.1 的帧格式被实测证实**：4 字节长度头精确等于 `帧总长-4`，且 §0.2 的"无填充、余数不处理"成立（266 = 14×19，整除，无尾部残留）。
2. **`0x0106` 未在 `Command.h` 中定义**（该域 `LOGON_USERCMD` 的 PARA 6 不存在），它是**服间内部心跳**，不在客户端 559 条表内。P2 需在 `msgid.csv` 补一条 `direction=internal` 的记录。
3. **内网链路不加密**（bit31=0），说明加密开关由连接类型决定，而不是全局；客户端连接才会置 ENC。这为服务端侧"能否明文解析"提供了判据。
4. **`dwTimestamp` 在内部链路是秒计数器**，不是 Unix 时间戳；客户端链路的该字段语义仍需客户端抓包判定，暂不合并结论。

复现：

```bash
python tools/remote.py capture 20 captures/server-internal-20s.pcapng "tcp and (port 10000 or port 8000)"
python tools/pcap_lite.py captures/server-internal-20s.pcapng --list
python tools/pcap_lite.py captures/server-internal-20s.pcapng --stream 0 --hexdump 96
```



## 1. 登录（login）

### 登录域（LOGON_USERCMD = 0x01）实测 12 条

| ID | 结构体 | 长度 | 说明 |
|---|---|---|---|
| `0x0101` | `stUserVerifyVerCmd` | 18 | 版本校验，payload 为 `"VER3.1.46.0\0"` |
| `0x0102` | `stUserRequestLoginCmd` | 40 | 账号密码登录 |
| `0x0103` | `stServerReturnLoginFailedCmd` | 15 | 登录失败 |
| `0x0104` | `stServerReturnLoginSuccessCmd` | 28 | 登录成功 |
| `0x0105` | `stPasswdLogonUserCmd` | 24 | 密码登录变体 |
| `0x0107` | `stAccountLogonUserCmd` | 16 | 账号登录变体 |
| `0x0109` | `stPasswordLogonUserCmd` | 17 | 密码登录变体 |
| `0x010A` | `stBackSelectUserCmd` | 14 | 返回选区 |
| `0x010B` | `stJpegPassportUserCmd` | — | 图片验证码 |
| `0x010C` | `stCountryInfoUserCmd` | — | 国家/区服信息 |

完整 559 条见 `msgid.csv`；C 结构体声明见 `schema/commands.h`。


### 0x???? LOGIN_VER (C→S)

<!-- TODO: 九列表 -->

### 0x???? LOGIN_AUTH (C→S)

<!-- TODO: 九列表 -->

### 0x???? LOGIN_RESULT (S→C)

<!-- TODO: 九列表 -->

## 2. 选区（select-server）

<!-- TODO: 服务器列表下发 / 选区请求 / 选区结果 -->

## 3. 进图（enter-map）

<!-- TODO: 角色列表 → 角色进入 → 场景加载完成 -->

## 4. 心跳（heartbeat）

<!-- TODO: 间隔 15 s 假设，待实测；超时判定 -->

## 5. 移动（move）

<!-- TODO: 坐标结构、朝向、速度、寻路点序列 -->

## 6. 背包（inventory）

<!-- TODO: 增删改、堆叠、使用、丢弃 -->

## 7. 异常与未知

| opcode | 现象 | 处理策略 |
|---|---|---|
| — | — | 丢弃 / 透传 / 仅日志 |
