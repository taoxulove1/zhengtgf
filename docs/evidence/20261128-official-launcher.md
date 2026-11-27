# 官方登录器（patchupdate.exe）登录链路改造 — 2026-11-28

目标：官方登录器 patchupdate.exe（源1，3.1.46.0 怀旧版）登录自建服务器 101.33.242.142。
前置：整栈原生 8 服务已通（main=1473d84），登录 8000 / 网关 6020 外网实测可达。

## 一、服务器侧 HTTP（80 端口）

- **80 外网探测为 Connection refused 而非超时** —— 安全组根本没封 80，只是没人监听。
  起 HTTP 后外网即刻可达，无需任何控制台操作。
- 服务：`python3 -m http.server 80`，目录 `/root/zt/www`，nohup 常驻（pid 由 boot 脚本托管）。
- 已伺服三路径副本（patchupdate.exe 字符串实测的三个请求路径）：
  - `/autopatch/*`（patchsetup.ini `path=` 指向）
  - `/autopatchhuaijiu/*`（怀旧版写死路径）
  - `/Zhengtu/Client/autopatchhuaijiu/*`（备用路径）
- 文件（本地生成器 `tools/gen_www_payload.py` → `dist-www/autopatch/`）：

| 文件 | 内容 |
|---|---|
| srvlist.xml | GBK；gametype=7、patch item type=101、zone name=雲天征途 id=1 addr=101.33.242.142 port=8000 type=710 newzone=1 |
| autoupdate.xml | 空 `<files></files>`（本服无需补丁） |
| flist.xml | 579 文件全量 CRC 清单（官方格式） |
| board.htm / logo.shtml | GBK 公告页（登录器内嵌页指向此处） |

- **flist CRC 算法破解**（对照官方怀旧服在线样本 `captures/official-flist.xml`，666 条）：
  zlib 多项式 0xEDB88320、init=0xFFFFFFFF、**无 final xor**，即 `zlib.crc32(data) ^ 0xFFFFFFFF`，
  格式 `%x` 不补零。全量 95 个同尺寸文件交叉验证 **94/95 命中**（唯一例外 patchupdate.exe
  系官方同尺寸替换过的二进制，正常漂移）。crc 属性顺序不统一（size/path/crc 可能换位）。
- 自启：`/root/native_boot.sh` 尾部 P3-WWW 段（80 未监听则拉起）。

## 二、patchupdate.exe 逆向结论（源1 580KB 原版，未加壳）

1. **列表主机与 patchsetup.ini 无关**。ini 读取键 = logo/board/home/pay/account/register/
   BugAddr0-1/BugPort0-1/bugobj/protectedAccountAddr（+OEM 扩展 forum/ad1-3/x_* 等）。
   给 ini 加 `update=`/`server=`/`servername=` 全部无效。
2. **列表主机 = 二进制内四级 fallback 链**（0x4169b2 起）：
   - 0x46ade8 + i*0x41c：配置数组，条目 0 = `211.159.201.33`、条目 1 = `172.17.102.60`（官方内网测试机）
   - 0x46c278：单串 `211.159.201.33`
   - 0x46ba40 / 0x46c694：`srvlistall.ztgame.com.cn`
   - 0x416780 = 对每个候选构造 HTTP 请求（条目布局：host[0..0x103]、port word@0x104、path@0x108，
     响应缓冲 0x100000 字节 @+0x1c0/+0x1d0），成功者胜出。
3. 请求路径：`/autopatchhuaijiu/srvlist.xml`、`/autopatch/srvlist.xml`、
   `/Zhengtu/Client/autopatchhuaijiu/srvlist.xml`，带 `?versionsign=%s`。
4. **update 键属于 config.ini**（`\\config.ini` + 长度 11 push 0xb）：读 `[update]` 节的
   NetType / showstartui / troykiller。第三方改版 config.ini 模板证实。
5. **注册表**：`RegOpenKeyExA(HKCU=0x80000001, "SOFTWARE\上海巨人网络科技有限公司\GiantIM")`
   → `RegQueryValueEx("InstallPath")` 取客户端目录。
6. 启动游戏命令行（第三方改版日志实证）：
   `data\zhengtu.dat -wd:<补丁URL> <ip1:ip2...> <port1:port2...> <type> <zone>`
7. patchupdate 的 epay 模块自更新走 epay\autoupdate.exe 子进程（Run 观测 epay 6 文件
   0:52:26 落盘即此机制）。

## 三、二进制 patch（tools/patch_patchupdate.py，幂等）

三个候选主机全为 14 字符，与 101.33.242.142 **等长无损替换**，共 6 处：
`srvlistall.ztgame.com.cn`×2 + `211.159.201.33`×2 + `172.17.102.60`×2。
备份：`captures/patchupdate.exe.bak-orig`。`--verify` 校验：new-hits=6、old-gone=True。

## 四、客户端最终配置

- `E:\源端\源1\patchupdate.exe` — patch 版（6 处指向 101.33.242.142）
- `patchsetup.ini` — logo/board/home/pay/account/register/protectedAccountAddr/BugAddr 全指向本服 80
- `config.ini` — [server] loginaddress=101.33.242.142 loginport=8000（客户端直连段）+
  [update] showstartui=0 nettype=0 troykiller=0（第三方实测模板）
- 注册表 `HKCU\SOFTWARE\上海巨人网络科技有限公司\GiantIM\InstallPath = E:\源端\源1`
- 桌面启动器（GBK 编码 bat，任何代码页可用）：
  - `zt-launcher.bat` → patchupdate.exe（官方登录器）
  - `zt-direct.bat` → `zhengtu.dat -wd:http://101.33.242.142/autopatch 101.33.242.142 8000 710 0`（直连）

## 五、第三方客户端清除（用户授权「可以删除」）

`D:\征途测\雲天征途_W10修复\雲天征途客户端\` 根目录 18 个第三方启动器文件
（龙跃征途.exe、雲天征途测试启动器.exe、11333333.exe、591Js-ZtGame.dat、ZtGame.dat、
改版 patchupdate.exe/ztconfig.exe、repair.exe、uninst.exe、Tapi32.dll、GetSafeBoxStatus.dll、
patchsetup.ini、startflist.xml、backupflist.xml、日志与 config.ini×2）→
备份移至 `E:\源端\backup\dclient-launchers-20261128\`。客户端数据目录（data/ 等）未动。
其 config.ini 模板与 patchupdate.log 已留档逆向用。

## 六、沙箱环境限制（本会话边界）

- patchupdate.exe 在 WorkBuddy 沙箱派生进程内：GUI 正常弹出，但 40s×34 次高频轮询
  **零 socket**（同环境 curl 走代理/直连均 200）。Run 观测到的「官方连接」实为 epay 子进程。
- zhengtu.dat 可 CreateProcess 启动（pid 存活 15s+），15-25s 间自杀——HProtect 内核驱动
  （ztsafe.sys/HPLoader.sys）在沙箱内加载失败。
- 结论：**端到端登录验证必须真实桌面**（HProtect 驱动 + patchupdate 网络行为两处环境依赖）。
  已交付桌面 bat，用户双击即测。

## 七、遗留

1. patchupdate.exe 在真实桌面的行为待验证（预期：fallback 链首候选已是本服 IP，直接命中
   `/autopatchhuaijiu/srvlist.xml` → 显示「雲天征途」区服 → 开始游戏 → zhengtu.dat 拿
   命令行参数连 8000）。
2. fallback 链 test/请求函数 0x416780 的完整语义未 100% 静态还原（COM/虚表调用较深），
   若真实桌面异常再攻。
3. srvlist zones 的 id/type 组合（710/101）若客户端校验版本号（versionsign），迭代看
   服务器 access log 请求参数即知。
4. HTTP 服务为 python3 单线程；多用户并发补丁下载建议后续换 nginx。

## 八、.co 补丁格式破解（20261128 02:15 定案）

**结论：`.co` = raw deflate（windowBits=-15），无 zlib 头，无任何加密。**

破解过程（含两次误判记录）：
1. 误判一：先入为主套客户端 XPAK 3DES 算法（20210112taozhi 密码）→ 全部解不开。
   根因排查中发现 cotool.cpp 的 `DES_LONG=unsigned long` 在 LP64 下为 8 字节，
   DES 半块读写全错——修复后仍解不开，证明算法本身对不上而非实现 bug。
2. patchupdate.exe 静态逆向：XPAK 魔数 0x46db24、pack-open 0x42c020（明文头+末尾索引）、
   KeyDerive 0x4291c0（密码 `xuzhao20041108` 硬编码于 .rdata 0x46db2c，15 字节，
   MD5String 0x44ee80 → 3DES HeaderData 表 0x481498 与客户端 HeaderData_DES_SPtrans
   逐字节一致）、写路径 0x42b69d。此机制用于**本地 `<名>_pack.pack` 缓存**
   （0x4198d0/0x41a410 的 `_pack`/`%s.pack` 命名），非服务器 .co。
3. 终判：官方配对样本 raw deflate 扫描一发命中——
   - official-flist.xml.co(11102B) → inflate → **59164B，与 official-flist.xml 逐字节一致**
   - official-patchsetup.ini.co(200B) → 398B ini 明文
   - official-ztsafe.sys.co(4698B) → 8976B MZ 驱动
   - sample-00257.grp.co(17.9MB) → 21467032B pack 体
4. URL 四模板（`%s.co`/`?time=`/`?CRC=`/`?versionsign=`，0x41b730）确认所有下载
   强制 .co 后缀、无明文回退；python http.server 会剥离 query string，天然兼容。

**生成与伺服**（tools/gen_co_files.py）：17+1 个 .co 已上传三路径
（/autopatch/、/autopatchhuaijiu/、/Zhengtu/Client/autopatchhuaijiu/）：
flist.xml.co、srvflist.xml.co、patchsetup.ini.co、patchupdate.exe.co、config.ini.co、
patchupdate.log.co(stub)、uninst.exe.co、Data|data/ztsafe.sys.co（官方 8976B 驱动）、
epay/{version.xml,autoupdate.exe,epayupdate.exe,cfg.ini,epay.exe,unzip.exe,
autoupdate.xml,register.exe,Register.exe}.co。
服务器端自验 18/18 OK（HTTP 200 + inflate 回验）；外网 curl flist.xml.co = 200/14758B。
本地 15 个 0 字节 .dl 残留已清。

工具沉淀：tools/cobf*.cpp（密码×表×skip 爆破，含 LP64 DES_LONG 修复）、
tools/usweep.cpp、tools/gen_co_files.py、tools/analyze_patchupdate_co.py、
tools/disasm_42c0.py、tools/disasm_keyderive.py、tools/find_callers.py。
