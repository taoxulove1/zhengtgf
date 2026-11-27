# 数据库基线 · MySQL

> 状态 **已验证结构，待迁移和联调**。本地 MySQL 4.0.17 已从空目录创建 Zebra 的 59 张表。目标 MySQL 5.7.44 的导入仍待验证。

## 本地材料核验

- 仓库的 `sql/schema.sql` 含 59 个建表定义。表名与旧库的 `.FRM/.MYD/.MYI` 三件套及部署记录逐项一致。
- 建表语句来自本地 `zebra_fixed2.sql` 与 `zebra_q2.sql` 两份转换结果。核对两份定义的列、索引和默认值后，采用带反引号的标识符写法，修复 `charbase` 中的 `INT` 列名导致的语法错误。脚本只建表，需先选用空的 `latin1` 测试库。
- 对三份本地 Zebra 快照运行 `myisamchk --read-only --check`，每份 59 张表均返回 0。每份均有 15 张表报告未正常关闭或仍处于使用状态，旧数据迁移前需要在独立副本上复验。
- 新端运行数据要求 62 个不同的 TBL 文件名，旧服务端散装数据只有 20 个同名候选。客户端的 `UpgradePurpleBase.tbl` CRC 验证通过，但其 12 字节记录与服务端的 116 字节结构不兼容。

## 1. 版本与字符集

| 项 | 值 | 确认 |
|---|---|---|
| 本地旧库验证版本 | MySQL 4.0.17 | 已实测 |
| 目标版本 | MySQL 5.7.44 | 待实测 |
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

## 4. 核心表

| 表名 | 用途 | 已验证 |
|---|---|---|
| `login` | 账号登录 | 结构创建 |
| `charbase` | 角色档案及名称 | 结构创建 |
| `zoneinfo` | 区服映射 | 结构创建 |

## 5. 校验方法

在空目录中使用 MySQL 4.0.17 的 `--bootstrap --skip-networking`。在测试输入中将 `ENGINE=MyISAM` 换成旧版接受的 `TYPE=MyISAM`。建表进程退出码为 0，生成了与清单同名的 59 组 `.frm/.MYD/.MYI`。

在目标 5.7.44 实例中选用空的 `latin1` 测试库，原样导入 `sql/schema.sql` 后执行下列检查。恢复旧数据后，以相同账号范围和角色 ID 对照中文名字节，记录实测差异。

```sql
SHOW TABLE STATUS;
SHOW CREATE TABLE charbase;
SELECT CHARID, HEX(NAME) FROM charbase ORDER BY CHARID LIMIT 20;
```

## 6. 回滚

在隔离测试实例中重新创建空测试库即可重做导入。正式库的备份和恢复演练、中文名字节对照、账号登录及角色列表读取均待验证。文件门禁通过只表示仓库已提交基线材料。
