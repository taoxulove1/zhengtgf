<#
.SYNOPSIS
    用 Windows 自带 pktmon 抓客户端启动流量，导出 pcapng。
.DESCRIPTION
    pktmon 需要管理员权限。本脚本会在提权不足时直接报错并给出一行提权命令。
    抓包时长内会启动客户端启动器（ztconfig.exe），覆盖 patchsetup 的连通性探测阶段。
.EXAMPLE
    # 在「以管理员身份运行」的 PowerShell 里执行
    pwsh -File tools/capture.ps1 -Seconds 20
#>
[CmdletBinding()]
param(
    [string]$ClientExe = "E:\源端\官方\ztconfig.exe",
    [int]$Seconds = 20,
    [string]$Out = "captures\boot.pcapng",
    [string]$Filter = ""
)

$ErrorActionPreference = "Stop"
$admin = ([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
if (-not $admin) {
    throw "需要管理员权限。右键 PowerShell → 以管理员身份运行，再执行本脚本。"
}

New-Item -ItemType Directory -Force -Path (Split-Path $Out -Parent) | Out-Null
$etl = [System.IO.Path]::ChangeExtension($Out, ".etl")

# 后端选择：装了 Wireshark/npcap 就用 dumpcap（pcapng 原生、字段更全），否则回退 pktmon
$dumpcap = (Get-Command dumpcap -ErrorAction SilentlyContinue)
if ($dumpcap) {
    Write-Host "[backend] dumpcap $($dumpcap.Source)"
    $iface = & $dumpcap.Source -D 2>$null | Select-String -Pattern "\\Device\\NPF_" | Select-Object -First 1
    if (-not $iface) { throw "dumpcap 未找到可用接口，确认 npcap 已安装" }
    $dev = ($iface.ToString() -split "\.")[0].Trim()
    $dargs = @("-i", $dev, "-w", $Out, "-a", "duration:$Seconds")
    if ($Filter) { $dargs += @("-f", $Filter) }
    $proc = Start-Process -FilePath $dumpcap.Source -ArgumentList $dargs -PassThru -NoNewWindow
    if (Test-Path $ClientExe) {
        Write-Host "      启动客户端 $ClientExe"
        $cp = Start-Process -FilePath $ClientExe -WorkingDirectory (Split-Path $ClientExe -Parent) -PassThru
    } else { $cp = $null }
    Start-Sleep -Seconds $Seconds
    if ($cp -and -not $cp.HasExited) { $cp | Stop-Process -Force -ErrorAction SilentlyContinue }
    if (-not $proc.HasExited) { $proc | Stop-Process -Force -ErrorAction SilentlyContinue }
    if (Test-Path $Out) {
        $f = Get-Item $Out
        $h = (Get-FileHash $Out -Algorithm SHA256).Hash.ToLower()
        Write-Host "      完成: $($f.FullName)  $([math]::Round($f.Length/1KB,1)) KB"
        Write-Host "      sha256=$h"
        Add-Content -Path (Join-Path (Split-Path $Out -Parent) "index.csv") -Value ("{0},{1},{2},{3}" -f (Split-Path $Out -Leaf), $f.Length, $h, (Get-Date -Format "yyyy-MM-ddTHH:mm:ssZ"))
    } else { throw "dumpcap 未产出文件" }
    return
}
Write-Host "[backend] pktmon（未检测到 dumpcap）"

Write-Host "[1/5] 清理旧会话"
pktmon stop 2>$null | Out-Null
pktmon filter remove 2>$null | Out-Null

if ($Filter) {
    Write-Host "[2/5] 添加过滤器 $Filter"
    pktmon filter add -i $Filter | Out-Null
} else {
    Write-Host "[2/5] 不加过滤器（全量）"
}

Write-Host "[3/5] 开始抓包 → $etl"
pktmon start --capture --pkt-size 0 --file-name $etl | Out-Null

if (Test-Path $ClientExe) {
    Write-Host "      启动客户端 $ClientExe"
    $p = Start-Process -FilePath $ClientExe -WorkingDirectory (Split-Path $ClientExe -Parent) -PassThru
} else {
    Write-Warning "客户端不存在: $ClientExe，仅抓本机流量"
    $p = $null
}

Start-Sleep -Seconds $Seconds

if ($p -and -not $p.HasExited) { $p | Stop-Process -Force -ErrorAction SilentlyContinue }

Write-Host "[4/5] 停止抓包"
pktmon stop | Out-Null

Write-Host "[5/5] 转 pcapng → $Out"
pktmon etl2pcap $etl -o $Out | Out-Null

if (Test-Path $Out) {
    $f = Get-Item $Out
    $h = (Get-FileHash $Out -Algorithm SHA256).Hash.ToLower()
    Write-Host "      完成: $($f.FullName)  $([math]::Round($f.Length/1KB,1)) KB"
    Write-Host "      sha256=$h"
    Add-Content -Path (Join-Path (Split-Path $Out -Parent) "index.csv") -Value ("{0},{1},{2},{3}" -f (Split-Path $Out -Leaf), $f.Length, $h, (Get-Date -Format "yyyy-MM-ddTHH:mm:ssZ"))
} else {
    throw "etl2pcap 失败，检查 pktmon 是否可用"
}
