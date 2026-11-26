@echo off
setlocal enabledelayedexpansion

:: 创建runclient目录
if not exist "runclient\" (
    mkdir "runclient"
    if errorlevel 1 (
        echo 创建runclient目录失败
        pause
        exit /b 1
    )
)

:: 遍历所有子目录的Release文件夹
for /d %%d in (*) do (
    if exist "%%~d\Release\" (
        echo 正在处理 %%~d\Release...
        
        :: 复制所有exe文件
        for %%f in ("%%~d\Release\*.exe") do (
            if exist "%%~f" (
                echo 复制 %%~f
                copy /y "%%~f" "runclient\"
                if errorlevel 1 (
                    echo 复制失败: %%~f
                )
            ) else (
                echo 文件不存在: %%~f
            )
        )
    ) else (
        echo 未找到Release目录: %%~d
    )
)

echo 所有exe文件已复制到runclient目录