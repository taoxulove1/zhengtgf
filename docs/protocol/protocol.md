# 协议文档 · 征途 3.1.46.0

> P2 已校验源码报文布局。客户端实测待 P5。
> 门禁：本文件 ≥ 120 行且六类消息齐全，否则 `P2` 不得置 done。

## 0. 报文封装（P1 源码与样本）

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

### 0.3 验证范围

仓库的帧层自检使用合成输入，客户端登录链路完整运行待验证。登录服源码在启用通信加密时有 RC5 路径。现有实包只支持下节的内部链路结论。

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
2. **`0x0106` 未在 `Command.h` 中定义**（该域 `LOGON_USERCMD` 的 PARA 6 不存在），它是**服间内部心跳**，不在客户端 559 条表内。内部心跳在第 4 节单列。
3. **内网链路不加密**（bit31=0）。这段抓包没有客户端连接。
4. **`dwTimestamp` 在内部链路是秒计数器**，不是 Unix 时间戳；客户端链路的该字段语义仍需客户端抓包判定，暂不合并结论。

复现：

```bash
python tools/pcap_lite.py captures/server-internal-20s.pcapng --list
python tools/pcap_lite.py captures/server-internal-20s.pcapng --stream 0 --hexdump 96
```

## 1. 登录（login）

下列偏移从去除传输层长度头后的消息开头起算。共同头为 6 字节，依次是命令域、子命令和 32 位时间字段。数值字段为小端。长度列对应当前源码的 32 位布局。客户端链路的运行证据尚待 P5。

| ID | 方向 | 长度规则 | 字段名 | 类型 | 字节序 | 偏移 | 取值样例 | 触发场景 |
|---|---|---|---|---|---|---:|---|---|
| 0x0101 | C→S | 10 | version | u32 | LE | 6 | 1999（源码默认） | 校验版本 |
| 0x0102 | C→S | 94 | pstrName | bytes[48] | — | 6 | 测试账号字段 | 提交账号认证 |
| 0x0102 | C→S | 94 | pstrPassword | bytes[16] | — | 54 | — | 提交账号认证 |
| 0x0102 | C→S | 94 | game / zone | u16 / u16 | LE | 70 / 72 | 0 / 1（示例） | 请求目标游戏区 |
| 0x0103 | S→C | 7 | byReturnCode | u8 | — | 6 | — | 认证或区服检查失败 |
| 0x0104 | S→C | 288 | pstrIP / wdPort | bytes[16] / u16 | LE 端口 | 14 / 30 | — | 返回网关地址 |

见 [三步登录时序](login_handshake.md) 和 [登录服源码](../../src/server/FLServer/LoginTask.cpp)。原索引列出的其他同域子命令保留为源码查找入口。

## 2. 选区（select-server）

登录请求已经包含目标游戏号和区号。连接网关后，创建角色界面的国家列表请求与初次选择区服属于两个操作。

| ID | 方向 | 长度规则 | 字段名 | 类型 | 字节序 | 偏移 | 取值样例 | 触发场景 |
|---|---|---|---|---|---|---:|---|---|
| 0x1807 | C→S | 6 | — | 共同头 | — | — | — | 请求国家列表 |
| 0x010C | S→C | 8 + 38 × size | size | u16 | LE | 6 | 1（示例） | 返回国家列表 |
| 0x010C | S→C | 8 + 38 × size | countryinfo | Country_Info[] | LE 数值 | 8 | — | 每条包含 ID、注册/登录状态和名称 |

[网关列表处理](../../src/server/GatewayServer/GatewayTask.cpp) 根据配置从角色存档服或会话服获取国家排序信息。部署环境的区服显示与可用状态在 P5 对照。

## 3. 进图（enter-map）

| ID | 方向 | 长度规则 | 字段名 | 类型 | 字节序 | 偏移 | 取值样例 | 触发场景 |
|---|---|---|---|---|---|---:|---|---|
| 0x1801 | S→C | 246 + size | charInfo | SelectUserInfo[2] | LE 数值 | 6 | — | 下发两个角色槽位 |
| 0x1801 | S→C | 246 + size | size | u16 | LE | 244 | 0（无图片） | 尾部附加的验证码字节数 |
| 0x1803 | C→S | 17 | charNo | u32 | LE | 6 | 角色数组下标 | 确认角色 |
| 0x031C | S→C | 192 + 12 × npc_count | width / height | u32 / u32 | LE | 6 / 10 | — | 下发地图尺寸 |
| 0x031C | S→C | 192 + 12 × npc_count | pstrFilename | bytes[32] | — | 46 | — | 提供地图文件名 |
| 0x031C | S→C | 192 + 12 × npc_count | mainRoleX / mainRoleY | i32 / i32 | LE | 182 / 186 | — | 下发角色入图坐标 |
| 0x031C | S→C | 192 + 12 × npc_count | npc_count / npc_list | i16 / 条目数组 | LE | 190 / 192 | 0–300（发送端限制） | 附加初始 NPC 的 ID、x、y |
| 0x031D | C→S | 6 | — | 共同头 | — | — | — | 确认地图加载 |
| 0x03FF | S→C | 6 | — | 共同头 | — | — | — | 通知初始数据发送结束 |

角色列表长度来自 [GatewayServer/GateUser.cpp](../../src/server/GatewayServer/GateUser.cpp)，地图长度来自 [ScenesServer/SceneUser.cpp](../../src/server/ScenesServer/SceneUser.cpp) 的实际发送表达式。

## 4. 心跳（heartbeat）

| ID | 方向 | 长度规则 | 字段名 | 类型 | 字节序 | 偏移 | 取值样例 | 触发场景 |
|---|---|---|---|---|---|---:|---|---|
| 0x0106 | 服间 S→S | 10 | 秒计数 | u32 | LE | 2 | 相邻帧递增 1 | 已抓包的内部心跳 |
| 0x0106 | 服间 S→S | 10 | 未知尾字段 | bytes[4] | — | 6 | 样本为 0 | 已抓包的内部心跳 |
| 0x0204 | C→S、S→C | 6 | — | 共同头 | — | — | — | 网关探测及回包 |

内网样本每秒一帧。客户端探测的间隔和超时要按客户端测试记录验收。

## 5. 移动（move）

| ID | 方向 | 长度规则 | 字段名 | 类型 | 字节序 | 偏移 | 取值样例 | 触发场景 |
|---|---|---|---|---|---|---:|---|---|
| 0x0602 | C→S、S→C | 16 | dwUserTempID | u32 | LE | 6 | — | 标识移动实体 |
| 0x0602 | C→S、S→C | 16 | byDirect | u8 | — | 10 | 0–7（八方向） | 行进方向 |
| 0x0602 | C→S、S→C | 16 | bySpeed | u8 | — | 11 | — | 速度字段 |
| 0x0602 | C→S、S→C | 16 | x / y | u16 / u16 | LE | 12 / 14 | — | 地图坐标 |

普通移动消息使用单组坐标，见 [场景服务处理](../../src/server/ScenesServer/SceneUser.cpp) 与 [客户端发送逻辑](../../src/client/Client/MainCharacter.cpp)。

## 6. 背包（inventory）

| ID | 方向 | 长度规则 | 字段名 | 类型 | 字节序 | 偏移 | 取值样例 | 触发场景 |
|---|---|---|---|---|---|---:|---|---|
| 0x0401 | S→C | 4397 | byActionType / object | u8 / t_Object | — | 6 / 7 | — | 下发新增或变更的道具记录 |
| 0x0402 | S→C | 10 | qwThisID | u32 | LE | 6 | — | 从客户端清除道具记录 |
| 0x0406 | S→C | 14 | qwThisID / dwNum | u32 / u32 | LE | 6 / 10 | — | 刷新堆叠数量 |
| 0x0407 | C→S | 14 | qwThisID / dwNumber | u32 / u32 | LE | 6 / 10 | — | 请求使用道具 |

物品记录的整体宽度已校验，内部字段沿用 [Object.h](../../src/server/base/Object.h)。道具数量变化及持久化属于 P5。

## 7. 版本与证据边界

- [schema/p2_wire.h](schema/p2_wire.h) 的核心结构用原源码做了 x86 编译器布局对照。
- [msgid.csv](msgid.csv)、[fields.csv](fields.csv) 和 [protocol.generated.md](protocol.generated.md) 保留旧版生成器的 559 条索引。旧生成器会把头部重复计入，并漏掉宏展开数组、复合类型和可变尾部。实现 P3 时，核心报文以本页与校验头文件为准，其余条目需回查源定义。
- 现有实包证据仅覆盖服间的 0x0106。登录、选区、进图、客户端心跳、移动、背包的方向和长度来自源码，客户端真机验收记入 P5。
