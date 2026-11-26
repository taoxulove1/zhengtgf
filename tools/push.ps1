<#
.SYNOPSIS
    绑定远端并首推 main。Token 从环境变量读，不落盘、不回显。

.EXAMPLE
    $env:GITHUB_TOKEN="ghp_xxx"; pwsh -File tools/push.ps1
#>
[CmdletBinding()]
param(
    [string]$Repo = "taoxulove1/zhengtugf",
    [string]$RemoteName = "origin",
    [string]$Token = $env:GITHUB_TOKEN
)

$ErrorActionPreference = "Stop"
Set-Location (Split-Path -Parent $PSScriptRoot)

if (-not $Token) { throw "缺 GITHUB_TOKEN：`$env:GITHUB_TOKEN='ghp_xxx'" }

$existing = git remote 2>$null
if ($existing -contains $RemoteName) {
    git remote remove $RemoteName
}
# 用 header 方式带 token，避免写入 .git/config
git -c "http.extraheader=Authorization: Bearer $Token" remote add $RemoteName "https://github.com/$Repo.git"

Write-Host "远端: https://github.com/$Repo.git"
Write-Host "分支: $(git branch --show-current)"
Write-Host "提交: $(git rev-list --count HEAD) 个 / 文件 $(git ls-files | Measure-Object).Count 个"

git -c "http.extraheader=Authorization: Bearer $Token" push -u $RemoteName main
if ($LASTEXITCODE -ne 0) { throw "推送失败：检查 token 权限与网络（代理 / 443 出网）" }

Write-Host "完成。接下来：`n  bash tools/protect-branch.sh"
