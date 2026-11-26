# 协议文档规范（P2 交付标准）

## 1. 文件清单

| 文件 | 内容 | 门禁 |
|---|---|---|
| `protocol.md` | 九列消息表，按业务分类分节 | ≥ 120 行；六类齐全 |
| `msgid.csv` | 机器可读消息索引 | ≥ 60 条 |
| `schema/*.h` | `#pragma pack(1)` 结构体声明 | 字段与表一一对应 |
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

| 规则 | 判定 | 解析方式 |
|---|---|---|
| `fixed(N)` | 每次长度恒为 N | 直接按结构体读 |
| `u16prefix` | 前 2 字节为包体长度（LE） | 读 2 字节 → 再读 N |
| `nul` | 以 `0x00` 结束 | 扫描到 0 为止 |

## 5. 编写约定

- 字段偏移必须**零重叠**，改表时同步改 `schema/*.h`。
- 未解出的字段标 `unknown`，不猜语义。
- 一条消息一个二级标题，标题格式 `### 0x0102 LOGIN_REQ (C→S)`。
- 提交信息：`docs(proto): 补 0x0102 登录请求字段`。

## 6. 当前状态

`protocol.md` 为骨架，六类均未填写；`msgid.csv` 仅表头。P1 未过门禁前不得开写。
