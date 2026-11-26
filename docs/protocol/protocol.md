# 协议文档 · 征途 3.1.46.0

> 状态：**骨架**（P2 未开始）。填写标准见 `README.md`。
> 门禁：本文件 ≥ 120 行且六类消息齐全，否则 `P2` 不得置 done。

## 0. 报文封装（P1 源码实测结论）

| 层 | 实测值 | 依据 |
|---|---|---|
| 传输 | TCP | `stNullUserCmd` 按流式解析 |
| 头部 | 6 字节固定：`BYTE byCmd` + `BYTE byParam` + `DWORD dwTimestamp` | `Command.h:261` |
| opcode | `(byCmd << 8) | byParam`，形如 `0x0101` | 559 条已提取，见 `msgid.csv` |
| 加密 | **裸 DES-ECB**，8 字节一块，无 IV、无链接、无填充 | `CDes.cpp:47 encdec()` |
| 尾部 | 不足 8 字节的余数**不加密**，明文保留 | `return nLen - offset` |
| 密钥 | 8 字节，`DES_set_key_checked` 校验弱密钥 | `CDes.cpp:34` |
| 压缩 | 无 zlib；另有 RLE 独立压缩（`RLEEncDec.h`），不在传输主链 | — |
| 长度 | 无显式长度前缀，按 opcode 查表定长 | 559 条中 `length_rule` 全为 `fixed(N)` |

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
