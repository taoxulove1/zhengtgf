# 数据库基线 · MySQL

> 状态：**骨架**（P4A 未开始）。原库未就位，以下为待填模板。
> 门禁：本文 ≥ 30 行且 `sql/schema.sql` ≥ 10 行。

## 1. 版本与字符集

| 项 | 值 | 确认 |
|---|---|---|
| 源库版本 | 待探测（`.frm` 头 / dump 注释头） | ☐ |
| 目标版本 | 5.7.44 | ☐ |
| 表字符集 | latin1 | ☐ |
| 连接字符集 | gbk | ☐ |
| `sql_mode` | `''` | ☐ |
| `lower_case_table_names` | 1 | ☐ |
| `max_allowed_packet` | 64M | ☐ |

## 2. 连接参数

```ini
[client]
default-character-set=gbk

[mysqld]
character-set-server=latin1
collation-server=latin1_swedish_ci
sql_mode=
lower_case_table_names=1
max_allowed_packet=64M
```

## 3. 账号

| 账号 | 用途 | 插件 | 权限 |
|---|---|---|---|
| `game` | 服务端 | `mysql_native_password` | 库级 DML |
| `gm` | 后台 | 同上 | 库级 ALL（仅 127.0.0.1） |
| `backup` | 冷备 | 同上 | SELECT, LOCK TABLES |

## 4. 表清单（待填）

| # | 表名 | 行数 | 字符集 | 说明 |
|---|---|---|---|---|
| 1 | — | — | — | 角色 |
| 2 | — | — | — | 账号 |
| 3 | — | — | — | 物品 |

## 5. 校验方法

```sql
SELECT HEX(name) FROM t_role LIMIT 20;   -- 与源库逐行比对
SHOW TABLE STATUS;                       -- 表数与引擎
```

## 6. 回滚

<!-- TODO: 冷备命令、恢复步骤、RTO 目标 -->
