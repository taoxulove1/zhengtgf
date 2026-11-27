# 场景服起不来：定性为 VMProtect 反调试误报（2026-11-27）

> 结论先行：`ScenesServer_r.exe` 起不来的原因**不是**网卡、端口、权限，也**不是** Wine 文件系统映射，
> 而是该 exe 带 **VMProtect 壳 + 反调试**，在 Wine 5.0.2 下被判定为「存在调试器」，
> 壳主动弹框中止进程。`System Error. Code: 5 (Access denied)` 是壳中止后业务层自己的兜底提示，
> 属于**次生现象**，沿这条线排错会一直空转。

## 1. 现象

| 观测点 | 结果 |
|---|---|
| `ss -lntp \| grep 6010` | 无监听 |
| `ps aux \| grep ScenesServer` | 若干 `<defunct>`（僵尸），说明进程起来后立刻退出、父进程未回收 |
| 窗口标题 | `ScenesServer(BuildTime: 2008-03-22 19:18:37)-(RunTime: …)` 能出现，随后弹出 `Error` / `Error occurred` / `SocketError` 三个对话框 |
| 业务日志 `ScenesServer_r_exe_*.log` | 只有 `myWinMain` 的 CallStack，`errorcode=00000000`（业务层自认为没出错） |
| 截图 | 见 `captures/20261127-scenes-vmprotect-block.png` |

## 2. 关键证据

### 2.1 弹框原文（截图 OCR 逐字）

```
The application has encountered a problem. We are sorry for the inconvenience.
[256] Debugger detected - please disable it and restart the application.
OK
```

旁边还有第二个框，首行 `System Access…`（截断），即业务层随后报的 `System Error. Code: 5. Access denied.`。

`[256]` = `0x100`，是 VMProtect 的固定错误码；`Debugger detected - please disable it and restart the application`
是该壳的标志性文案。业务代码不会用这种句式，也不会有 `[256]` 这种错误码编号。

### 2.2 Wine backtrace 指向「弹框时崩溃」，不是业务崩溃

```
user32.dll.so(MessageBoxA+0x39)
[0xa406df]                          <- 壳代码调用 MessageBoxA
[0xa9652f]
...
winex11.drv.so(X11DRV_WindowPosChanging+0x3d0)
libX11.so.6(XOpenIM+0x47)
libX11.so.6(_XimLocalOpenIM+0x413)  <- 在这里 SIGSEGV
```

调用链是**壳 → MessageBoxA → winex11 建窗 → XOpenIM 崩**。
所以 `c0000005` 是 Wine 的 X11 输入法（locale 数据缺失）问题，
恰好发生在弹「Debugger detected」那个框的时候。**壳已经拒绝了这次启动**。

修复办法（只为看清弹框，不解决问题）：`export LANG=C LC_ALL=C` 后重跑，XIM 不再崩，
弹框能正常显示并被 `xdotool` 读到标题（已实测到 `Error` / `Error occurred` / `SocketError`）。

### 2.3 PE 特征：两个 exe 都被加壳，场景服更重

`python3 tools/pe_scan.py ScenesServer_r.exe FlServer_r.exe`（服务器实测输出）：

| exe | 大小 | 整体熵 | 节 | 导入 DLL | 入口所在节 |
|---|---|---|---|---|---|
| `ScenesServer_r.exe` | 2,985,984 | 7.42 | 9 节，其中 7 节 `raw=0`（运行期展开），节名乱码 `v5hiv27x` / `i3a6e364` / `60v.3bhx` / `3tj0bwlt` | **2 个**：`kernel32` `user32` | `60v.3bhx`（熵 7.61） |
| `FlServer_r.exe` | 3,284,992 | 6.60 | 7 节，`.text`/`.rdata` 正常，仅末尾一节乱码 `tÉmð£uö` | 8 个：`KERNEL32` `USER32` `GDI32` `ADVAPI32` `COMCTL32` `WINMM` `iphlpapi` `WS2_32` | `tÉmð£uö` |

判读：

- 乱码节名 + 7 个 `raw=0` 的「虚假节」+ 导入表只剩 `kernel32/user32` = 典型 **VMProtect 全量压缩**；
- `FlServer` 只压了最后一段，业务代码基本裸露 → 它能在 Wine 下跑起来；
- `ScenesServer` 全量压缩 → 每次启动都过一遍壳的完整性/反调试检查 → 在 Wine 下必挂。
- `md5`: `ScenesServer_r.exe = 13baf051c8ad563e636ad9f0655d997f`，`FlServer_r.exe = 0156dbc8adecaefe121fa77afaa6d819`（用于核对换包）。

## 3. 已排除项

| 假设 | 排除依据 |
|---|---|
| 端口 6010 被占 | `ss -lntp` 无占用；换端口一样退 |
| 需要 `xdotool` 点工具栏才 bind（其他服如此） | 场景服连窗口都留不住，进程在点之前就退了 |
| 权限 / 目录不可写 | `code:5` 出现在弹框之后，是壳中止的次生提示；目录 root 可写 |
| Wine 文件系统映射 | `Z:\root\zt\server` 映射正常，`CreateFileW ScenesServer_r.exe → 0x128` 成功，PE 头读得到（`WINEDEBUG=+file` 全程无 `ERROR_ACCESS_DENIED`） |
| 装错 wine 版本跑不起来 | 一开始用 `/usr/bin/wine`（4.0.4）报 `version mismatch 595/571`；换 `/opt/wine32/bin/wine`（5.0.2）后能加载、能进壳、才暴露本条 |

## 4. 三条路线与取舍

| 路线 | 做法 | 成本 | 判断 |
|---|---|---|---|
| A. 换 wine 版本 | VMProtect 反调试对 wine 版本极敏感，试 4.0.4 / 6.x / 7.x / 8.x，每版一个独立 `WINEPREFIX` | 每版 20 分钟 | **先试**，便宜；已试 4.0.4 独立前缀（`/root/.wine-v4`），进程仍秒退，需继续向上试 6+ |
| B. 脱壳 / patch 掉反调试 | VMProtect 虚拟化段内定位检查点，改跳转 | 极高（虚拟化段几乎无静态特征） | 不作为主路线 |
| C. **原生编译 ScenesServer** | 用 `/root/p3-test` 下已有源码直接 gcc 编出无壳 ELF/PE，绕开整个壳 | 中（编译错误可枚举） | **主路线**，见 §5 |

路线 C 已经有大量铺垫：`/root/p3-test/` 下有 `p3-scenes-source-fix/`、`p3-scenes-boost-fix/`
两套打过补丁的源码树与 `Makefile`，构建日志在做增量收敛。

## 5. 原生编译现状（主任务的实际断点）

最后一次构建（`p3-build-scenes-boost.log`）失败点全部是**新 GCC 的严格性**，不是逻辑错误：

```
../base/zAStar.h:172  error: declaration of 'const int radius'
../base/zAStar.h:173  error: shadows template parm 'int radius'
../base/zAStar.h:179  error: declaration of 'const int step'
../base/zAStar.h:180  error: shadows template parm 'int step'
Scene.h:175           error: invalid conversion from 'const char*' to 'char*' [-fpermissive]
FuBenMgr.cpp:…        error: …
make: *** [FuBenMgr.o] Error 1
```

对应的处置（按顺序做，每轮只推进一层，避免一次改太多无法定位）：

1. 加宽松开关：`-fpermissive -std=gnu++03 -fno-strict-aliasing -Wno-narrowing`
   —— 可一次消掉 `const char*→char*`、窄化、部分重载歧义。
2. `zAStar.h` 的模板参数遮蔽（`radius`/`step` 与成员默认实参同名）只能改源码：
   把模板形参改名 `RADIUS`/`STEP`，或把成员默认实参改成 `::radius` 不成立的写法 →
   实务做法是把成员函数签名里的默认值去掉，默认值放到调用点。
3. `Scene.h:175` 的 `strstr` 返回值赋给 `char*`：加 `const_cast<char*>` 或把 `real` 改 `const char*`。
4. `log4cxx` / `lua` / `luabind` 三条依赖已有单独日志（`p3-build-log4cxx.log`、`p3-build-lua.log`、`p3-build-luabind.log`），
   说明依赖侧已打过一轮；若继续报错，优先用系统 `-llua -llog4cxx` 而不是自带源码编译。
5. 编过 `ScenesServer` 后，端口不要用 P3 文档里的 7001/7002 —— **以实机为准：6010**，
   配置从 `/root/zt/server/Config.ini` 复制，改 `ListenPort` 与 Session 地址即可。

## 6. 复现命令（服务器侧）

```bash
# 1) 看清弹框（避免 XIM 崩溃遮住真相）
export WINEPREFIX=/root/.wine32 DISPLAY=:99 LANG=C LC_ALL=C WINEDEBUG=-all
cd /root/zt/server && /opt/wine32/bin/wine ScenesServer_r.exe &
sleep 8
xdotool search --onlyvisible --name "" | while read w; do xdotool getwindowname "$w"; done

# 2) 抓 PE 加壳特征（判断换包是否换成同样加壳的）
python3 /root/pe_scan.py /root/zt/server/ScenesServer_r.exe

# 3) 一键探针（起进程 + 列窗口 + 看 6010 + 截图）
bash /root/scene_probe.sh
```

仓库内对应工具：`tools/scene_probe.sh`、`tools/pe_scan.py`。

## 7. 待办（按优先级）

1. 试 wine 6.x/7.x/8.x 独立前缀跑原版 `ScenesServer_r.exe`，成本低先做；不通则死心走原生编译。
2. 原生编译收敛：按 §5 顺序清编译错误，产出可执行的 `ScenesServer`，直接监听 6010。
3. 原生版起来后核对：能否被 `GatewayServer` 连上（Session 握手）、`log/scenesserver_21` 是否开始写新日期目录。
4. 若最终必须用带壳原版：记录为已知缺陷，交付说明里写明「场景服不可用 → 只能单地图」，不要留悬案。
