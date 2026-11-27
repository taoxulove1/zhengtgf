# win_capture.ps1 —— 在 Windows 宿主机上用 Wireshark 自带 dumpcap 抓「官方客户端 <-> 自建服」的完整交互
#
# 为什么在客户端侧抓：原版服务端在 Wine 下只能给出原生回包，服务端侧 pcap 少了"客户端实际发了什么、
# 收到后又做了什么"这一半。客户端侧抓到的双向流才是 P3/P5 的 golden。
#
# 用法（不需要管理员，Npcap 1.89 已允许非管理员抓包）：
#   powershell -ExecutionPolicy Bypass -File tools\win_capture.ps1 -Seconds 300
#   powershell -ExecutionPolicy Bypass -File tools\win_capture.ps1 -Seconds 600 -Filter "host 101.33.242.142 or port 6020"
#
# 产物默认落在仓库 captures\win-client-<时间戳>.pcapng

[CmdletBinding()]
param(
    [int]$Seconds = 300,
    [string]$Filter = "host 101.33.242.142",
    [string]$Device = "",
    [string]$OutDir = ""
)

$ErrorActionPreference = "Stop"

# 1) 找 dumpcap：先注册表记录的安装位置，再常见路径
$ws = $null
$keys = @(
    'HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\*',
    'HKLM:\SOFTWARE\WOW6432Node\Microsoft\Windows\CurrentVersion\Uninstall\*'
)
$entry = Get-ItemProperty $keys -ErrorAction SilentlyContinue |
         Where-Object { $_.DisplayName -like 'Wireshark*' } | Select-Object -First 1
if ($entry -and $entry.InstallLocation -and (Test-Path (Join-Path $entry.InstallLocation 'dumpcap.exe'))) {
    $ws = $entry.InstallLocation
}
if (-not $ws) {
    foreach ($p in @("$env:ProgramFiles\Wireshark", "${env:ProgramFiles(x86)}\Wireshark")) {
        if ($p -and (Test-Path (Join-Path $p 'dumpcap.exe'))) { $ws = $p; break }
    }
}
if (-not $ws) { throw "找不到 dumpcap.exe，请确认 Wireshark 已安装" }
$dumpcap = Join-Path $ws 'dumpcap.exe'
Write-Host "dumpcap: $dumpcap"

# 2) 选网卡：默认取有默认路由的那块，再按 GUID 映射到 NPF 设备名
if (-not $Device) {
    $route = Get-NetRoute -DestinationPrefix '0.0.0.0/0' -ErrorAction SilentlyContinue |
             Sort-Object RouteMetric | Select-Object -First 1
    if (-not $route) { throw "没有默认路由，无法判断用哪块网卡" }
    $ad = Get-NetAdapter -InterfaceIndex $route.ifIndex
    if (-not $ad.InterfaceGuid) { throw "网卡 $($ad.Name) 没有 GUID" }
    $Device = "\Device\NPF_$($ad.InterfaceGuid)"
    Write-Host "网卡: $($ad.Name)  device=$Device"
}

# 3) 输出文件名
if (-not $OutDir) {
    $OutDir = Join-Path (Split-Path -Parent $PSScriptRoot) 'captures'
}
New-Item -ItemType Directory -Force -Path $OutDir | Out-Null
$stamp = Get-Date -Format 'yyyyMMdd-HHmmss'
$out = Join-Path $OutDir "win-client-$stamp.pcapng"

Write-Host "过滤: $Filter"
Write-Host "时长: $Seconds s"
Write-Host "输出: $out"
Write-Host "现在请启动官方客户端（龙跃征途.exe），登录到失败为止也会留下完整交互"

& $dumpcap -i $Device -f $Filter -a duration:$Seconds -w $out
$rc = $LASTEXITCODE
Write-Host "dumpcap 退出码: $rc"
if (Test-Path $out) {
    $f = Get-Item $out
    Write-Host ("完成: {0}  {1} B" -f $f.FullName, $f.Length)
}
exit $rc
