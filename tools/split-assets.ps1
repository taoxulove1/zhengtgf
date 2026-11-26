<#
.SYNOPSIS
    按 assets/manifest/assets.json 把 >=10 MB 的资源切成 90 MB 分卷。

.DESCRIPTION
    GitHub 单文件硬限 100 MB，因此 >=100 MB 的文件必须切分；10–100 MB 的
    可视配额走 LFS，此处一并切分以便统一用 Release 分发。
    产出：
      dist/<相对路径>/<原名>.part001 ...
      dist/parts.json            每卷的字节范围与 sha256
      dist/SHA256SUMS.txt        原文件整体哈希（供合并后校验）

.EXAMPLE
    pwsh -File tools/split-assets.ps1 -Source "E:/源端/官方" -Out dist -PartMB 90
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)][string]$Source,
    [string]$Out = "dist",
    [int]$PartMB = 90,
    [string]$Manifest = "assets/manifest/assets.json",
    [switch]$DryRun
)

$ErrorActionPreference = "Stop"
$partBytes = [int64]$PartMB * 1MB

if (-not (Test-Path $Manifest)) { throw "清单不存在: $Manifest（先跑 tools/gen-manifest.py）" }
$doc = Get-Content -Raw -Encoding UTF8 $Manifest | ConvertFrom-Json

$targets = $doc.files | Where-Object { $_.route -eq "release-split" -or $_.route -eq "lfs-or-release" }
Write-Host ("待切分 {0} 个文件，单卷 {1} MB" -f $targets.Count, $PartMB)

New-Item -ItemType Directory -Force -Path $Out | Out-Null
$index = @()

foreach ($f in $targets) {
    $src = Join-Path $Source ($f.path -replace "/", "\")
    if (-not (Test-Path $src)) {
        $src = Join-Path $Source (Split-Path $f.path -Leaf)
    }
    if (-not (Test-Path $src)) { Write-Warning "缺失，跳过: $($f.path)"; continue }

    $dir = Join-Path $Out (Split-Path $f.path -Parent)
    if ($dir) { New-Item -ItemType Directory -Force -Path $dir | Out-Null }
    $leaf = Split-Path $f.path -Leaf
    $size = (Get-Item $src).Length
    $parts = [math]::Ceiling($size / $partBytes)
    Write-Host ("  {0}  {1:N1} MB → {2} 卷" -f $leaf, ($size / 1MB), $parts)

    if ($DryRun) { continue }

    $fs = [System.IO.File]::OpenRead($src)
    try {
        $buf = New-Object byte[] $partBytes
        for ($i = 1; $i -le $parts; $i++) {
            $partName = "{0}.part{1:D3}" -f $leaf, $i
            $partPath = Join-Path $dir $partName
            $read = 0
            $offset = 0
            $pfs = [System.IO.File]::Create($partPath)
            try {
                while ($offset -lt $partBytes) {
                    $n = $fs.Read($buf, $offset, [int]($partBytes - $offset))
                    if ($n -le 0) { break }
                    $pfs.Write($buf, $offset, $n)
                    $offset += $n
                    $read += $n
                }
            } finally { $pfs.Close() }

            $index += [pscustomobject]@{
                file   = $f.path
                part   = $partName
                bytes  = $read
                sha256 = (Get-FileHash -Algorithm SHA256 -Path $partPath).Hash.ToLower()
            }
        }
    } finally { $fs.Close() }
}

if (-not $DryRun) {
    $index | ConvertTo-Json -Depth 4 | Set-Content -Encoding UTF8 (Join-Path $Out "parts.json")
    $doc.files | Where-Object { $_.route -eq "release-split" -or $_.route -eq "lfs-or-release" } |
        ForEach-Object { "{0}  {1}" -f $_.sha256, $_.path } |
        Set-Content -Encoding UTF8 (Join-Path $Out "SHA256SUMS.txt")
    Write-Host ("完成：{0} 卷，索引 {1}" -f $index.Count, (Join-Path $Out "parts.json"))
}
