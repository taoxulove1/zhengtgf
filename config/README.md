# config 目录

三处配置的分工，别混：

| 路径 | 性质 | 谁维护 | 是否随客户端发布 |
|---|---|---|---|
| `assets/client/` | **原样镜像**官方客户端里的文本配置（169 个），用于比对与还原 | 文定协 | 是 |
| `config/client/` | **生效基线**：从 assets 抽出的关键几项，带注释与版本锁定 | 文定协 | 否 |
| `config/server/` | 服务端配置模板，端口 / 路径 / 库连接，落地时回填 | 盛久稳 / 褚安库 | 否 |

## 硬规则

- 明文口令一律写 `${ENV_NAME}`，真实值走 `.env`（已 gitignore）或 Actions Secrets。
- 模板里出现的端口必须与 `docs/architecture.md` §3 端口表一致，改一处同步改另一处。
- `config/client/imversion.ini` 的 `version` 是版本校验真源，客户端与服务端两侧必须相等。
