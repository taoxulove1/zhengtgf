# 数据库基线 · MySQL

> 状态 **已验证 MySQL 4.0.17 快照恢复**。59 张表、1,030 行和两条角色名均通过对照。目标 MySQL 5.7.44 导入与服务端联调待验证。

## 本地材料核验

- 仓库的 `sql/schema.sql` 含 59 个建表定义。表名与旧库的 `.FRM/.MYD/.MYI` 三件套及部署记录逐项一致。
- 建表语句来自本地 `zebra_fixed2.sql` 与 `zebra_q2.sql` 两份转换结果。核对两份定义的列、索引和默认值后，采用带反引号的标识符写法，修复 `charbase` 中的 `INT` 列名导致的语法错误。脚本只建表，需先选用空的 `latin1` 测试库。
- 对三份本地 Zebra 快照运行 `myisamchk --read-only --check`，每份 59 张表均返回 0。每份均有 15 张表报告未正常关闭或仍处于使用状态。本次恢复验证使用 `MYSQL/data/Zebra` 的独立副本。
- 客户端的 `UpgradePurpleBase.tbl` CRC 验证通过，但其 12 字节记录与服务端的 116 字节结构不兼容。启动所需 TBL 和地图的剩余缺口见 [场景资源预检](../deploy/runbook.md#场景资源预检)。

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
| `login` | 账号登录 | 结构与记录恢复 |
| `charbase` | 角色档案及名称 | 记录内容、名字节与最后活跃时间 |
| `zoneinfo` | 区服映射 | 结构与记录恢复 |

## 5. 校验方法

### MySQL 4.0.17 快照恢复实测

从旧 MySQL 安装目录的 `data/Zebra` 读取快照，在两个临时目录中分别导出与恢复。全部 MySQL 进程使用 `--bootstrap --skip-networking`。脚本只在旧版测试输入中将 `ENGINE=MyISAM` 改为 `TYPE=MyISAM`。

```powershell
python tools/verify_zebra_restore.py --legacy-root 'E:\源端\BY\toolchain\work\02_dump\xp_server\MYSQL'
```

本次退出码为 0，输出 `Verified 59 tables, 1030 rows and 2 role names.`。核验对每张表的所有仓库定义字段比较行多重集，另按角色 ID 核对两条角色名的 `HEX(NAME)`。脚本只输出汇总，临时库与导出文件在退出后清理。

三份旧 SQL 转存的行数与快照全部一致，内容仍有差异。最接近原库的 `zebra_q.sql` 保留了角色名字节，却丢失了两条 `CHARBASE.LASTACTIVEDATE` 的原时间值。旧版默认导出会把这两条记录输出为 20 位数字，直接导入后变成零时间。经 `DATE_FORMAT` 输出标准日期时间再导入，按 `UNIX_TIMESTAMP` 比对确认两条值一致。

### 目标库验收

在目标 5.7.44 实例中选用空的 `latin1` 测试库，原样导入 `sql/schema.sql` 后执行下列检查。恢复时使用与导出一致的会话时区，再按角色 ID 对照名称和时间值。

```sql
SHOW TABLE STATUS;
SHOW CREATE TABLE charbase;
SELECT CHARID, HEX(NAME), UNIX_TIMESTAMP(LASTACTIVEDATE) FROM charbase ORDER BY CHARID LIMIT 20;
```

## 6. 回滚

本地验证从新的临时目录运行即可重复。目标环境的备份和恢复演练、账号登录及角色列表读取仍待验证。
