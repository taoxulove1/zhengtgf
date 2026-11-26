<#
.SYNOPSIS
    从 Release 分卷还原 assets/paks，并按清单校验 sha256。

.EXAMPLE
    pwsh -File tools/fetch-assets.ps1 -Tag v0.1.0 -Target assets/paks
    pwsh -File tools/fetch-assets.ps1 -Tag v0.1.0 -Target assets/paks -SkipDownload
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)][string]$Tag,
    [string]$Target = "assets/paks",
    [string]$Manifest = "assets/manifest/assets.json",
    [string]$Repo = "taoxulove1/zhengtugf",
    [switch]$SkipDownload
)

$ErrorActionPreference = "Stop"
$doc = Get-Content -Raw -Encoding UTF8 $Manifest | ConvertFrom-Json
$todo = $doc.files | Where-Object { $_.route -eq "release-split" -or $_.route -eq "lfs-or-release" }
Write-Host ("清单待还原 {0} 个文件" -f $todo.Count)

if (-not $SkipDownload) {
    if (-not (Get-Command gh -ErrorAction SilentlyContinue)) { throw "需要 gh CLI：winget install GitHub.cli" }
    New-Item -ItemType Directory -Force -Path $Target | Out-Null
    Push-Location $Target
    try {
        gh release download $Tag --repo $Repo --pattern "*.part*" --clobber
        gh release download $Tag --repo $Repo --pattern "parts.json" --clobber
    } finally { Pop-Location }
}

foreach ($f in $todo) {
    $leaf = Split-Path $f.path -Leaf
    $parts = Get-ChildItem -Path $Target -Filter "$leaf.part*" -Recurse | Sort-Object Name
    if (-not $parts) { Write-Warning "无分卷，跳过: $leaf"; continue }
    $dest = Join-Path $Target $f.path
    New-Item -ItemType Directory -Force -Path (Split-Path $dest -Parent) | Out-Null
    $out = [System.IO.File]::Create($dest)
    try {
        foreach ($p in $parts) {
            $in = [System.IO.File]::OpenRead($p.FullName)
            try { $in.CopyTo($out) } finally { $in.Close() }
        }
    } finally { $out.Close() }

    $hash = (Get-FileHash -Algorithm SHA256 -Path $dest).Hash.ToLower()
    if ($hash -eq $f.sha256) {
        Write-Host ("  OK  {0}" -f $f.path)
        $parts | Remove-Item -Force
    } else {
        Write-Warning ("哈希不符 {0}`n  期望 {1}`n  实际 {2}" -f $f.path, $f.sha256, $hash)
    }
}
