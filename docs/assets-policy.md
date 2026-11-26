# 数据分级与上传约定

> 结论先行：6.1 GB 客户端**不能整体入库**。GitHub 硬限单文件 100 MB、单次推送 2 GB、仓库建议 < 1 GB。按体积四层分流，仓库只保留文本、清单与源码。

---

## 1. 分层阈值

| 层 | 体积区间 | 落位 | 入库 | 校验方式 |
|---|---|---|---|---|
| A | < 1 MB | `assets/client/<原相对路径>` | 是 | git blob + sha256 |
| B | 1 MB – 10 MB | `assets/client/...`（可纳管） | 是（可选 LFS） | sha256 |
| C | 10 MB – 100 MB | Git LFS 或 Release 单卷 | 视配额 | sha256 |
| D | ≥ 100 MB | Release 90 MB 分卷 | 否 | sha256 + 卷号 |

`assets/manifest/assets.csv` 的 `route` 列即归属层，数值来自实测。

---

## 2. 当前实测分布

| route | 文件数 | 体积 |
|---|---|---|
| git | 442 | 60.5 MB |
| git-optional | 70 | 219.4 MB |
| lfs-or-release | 55 | 2151.2 MB |
| release-split | 15 | 3855.4 MB |
| **合计** | **582** | **6286.5 MB** |

> 生成命令：`python tools/gen-manifest.py --source "E:/源端/官方" --out assets/manifest`

---

## 3. Release 分卷规则

- 卷大小 **90 MB**（100 MB 硬限留 10 MB 余量）。
- 命名：`<原名>.partNNN`，三位序号，从 `001` 起。
- 附 `SHA256SUMS.txt`（整文件哈希）+ `parts.json`（每卷哈希与字节范围）。
- 合并：`cat Body1.pak.part* > Body1.pak`（Linux）／`tools/fetch-assets.ps1`（Windows）。

---

## 4. 上传流程

```
A/B 层  → git add → PR(data:change) → 2 人复核 → squash 合并
C/D 层  → tools/split-assets.ps1 切卷 → gh release upload <tag> *.part* →
          更新 assets/manifest/* → PR(data:manifest)
```

## 5. 更新触发

| 事件 | 动作 |
|---|---|
| 客户端补丁更新 | 重跑 `gen-manifest.py` → 差异写 `docs/patches/` → 发新 Release |
| 新增地图 / 模型 pak | 同上，单独立 PR，`data:change` 标签 |
| 仅改文本配置 | 直接 PR，无需 Release |
| 清单与实物不符 | `verify-assets.py` 报错 → 阻断合并 |
