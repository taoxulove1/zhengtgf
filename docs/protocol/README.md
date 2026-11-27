# 协议文档规范（P2 交付标准）

## 1. 文件清单

| 文件 | 内容 | 门禁 |
|---|---|---|
| `protocol.md` | 九列消息表，按业务分类分节 | ≥ 120 行；六类齐全 |
| `msgid.csv` | 机器可读消息索引 | ≥ 60 条 |
| `schema/p2_wire.h` | P2 核心消息的定长结构及变长前缀 | 与服务端源码的 x86 大小及字段偏移逐项对照 |
| `login_handshake.md` | 登录三步时序 + 字段偏移 | 三步齐全 |

## 2. `protocol.md` 九列

| 列 | 含义 | 示例 |
|---|---|---|
| ID | opcode，十六进制 | `0x0102` |
| 方向 | `C→S` / `S→C` | `C→S` |
| 长度规则 | `fixed(32)` / `u16prefix` / `nul` | `u16prefix` |
| 字段名 | 英文小写下划线 | `account_name` |
| 类型 | `u8/u16/u32/i32/char[N]/bytes` | `char[32]` |
| 字节序 | `LE` / `BE` | `LE` |
| 偏移 | 相对包体起始，从 0 计 | `4` |
| 取值样例 | 实测值 | `test_01` |
| 触发场景 | 何时发 | 登录第 2 步 |

## 3. `msgid.csv` 列

```csv
id,direction,name,length_rule,min_len,max_len,phase,captured
0x0102,C->S,LOGIN_REQ,u16prefix,44,44,login,yes
```

- `id` 统一 `0x` 前缀四位十六进制。
- `min_len` / `max_len` 为包体长度（不含 TCP 头）。
- `captured=yes` 表示有 pcap 佐证；`no` 表示仅从源码推断，需 P5 复验。

## 4. 长度规则三分类

> **注意层级**：下表描述的是**帧内单条消息**的长度规则。帧本身另有 4 字节长度头（bit31 加密 / bit30 压缩 / bit23-0 长度），详见 `protocol.md §0.1`。解码顺序是**先解密 → 再切帧 → 再按 opcode 表定长读**，不要跳过帧层直接按 opcode 扫流。

| 规则 | 判定 | 解析方式 |
|---|---|---|
| `fixed(N)` | 每次长度恒为 N | 直接按结构体读 |
| `u16prefix` | 前 2 字节为包体长度（LE） | 读 2 字节 → 再读 N |
| `nul` | 以 `0x00` 结束 | 扫描到 0 为止 |

现有 559 条中 `length_rule` 全为 `fixed(N)`，即帧内是多条定长消息首尾相接；帧尾 0~7 字节的 8 对齐填充不属于任何消息，解析器必须容忍。

## 5. 编写约定

- 字段偏移必须**零重叠**，改表时同步改 `schema/*.h`。
- 未解出的字段标 `unknown`，不猜语义。
- 一条消息一个二级标题，标题格式 `### 0x0102 LOGIN_REQ (C→S)`。
- 提交信息：`docs(proto): 补 0x0102 登录请求字段`。

## 6. 当前状态

- `msgid.csv`：**559 条**（47 个命令域），由 `tools/extract_msgid.py` 从 `src/server/base/Command.h` 自动提取。
- [协议正文](protocol.md) 已覆盖登录、选区、进图、心跳、移动和背包，三步时序见 [登录说明](login_handshake.md)。
- [P2 核心结构](schema/p2_wire.h) 已与原声明做 x86 编译器布局核验。变长前缀的长度公式见协议正文。
- `schema/commands.h`、`fields.csv` 和 `protocol.generated.md` 是早期生成器的检索材料。生成器存在重复计入命令头和漏算复合字段的问题。
- 入库抓包覆盖内部心跳。客户端消息的运行状态在 P5 对照验证。
