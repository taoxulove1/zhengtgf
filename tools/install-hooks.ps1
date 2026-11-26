<#
.SYNOPSIS
    安装本地 git 钩子（pre-push 拦截直推 main / 分支名 / 提交信息）。
.EXAMPLE
    pwsh -File tools/install-hooks.ps1
#>
$ErrorActionPreference = "Stop"
Set-Location (Split-Path -Parent $PSScriptRoot)

git config core.hooksPath .githooks
if ($LASTEXITCODE -ne 0) { throw "git config 失败" }

# Git for Windows 下 sh 脚本需可执行位
$hook = ".githooks/pre-push"
if (Test-Path $hook) {
    git update-index --add --chmod=+x $hook 2>$null
}

Write-Host "hooksPath = $(git config --get core.hooksPath)"
Write-Host "已启用：拒绝直推 main、分支名白名单、提交信息规范"
Write-Host "确需直推时：`$env:ALLOW_MAIN_PUSH='1'; git push"
