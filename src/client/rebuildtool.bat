@echo off

::call "C:\Program Files (x86)\Microsoft Visual Studio .NET 2003\Common7\Tools\vsvars32.bat"

set start_time=%time%
devenv "MakeGLNew/MakeGL.sln" /rebuild Release
set end_time=%time%

echo 开始时间: %start_time%
echo 结束时间: %end_time%

setlocal enabledelayedexpansion
set /a "start_secs=((1%start_time:~0,2%-100)*3600)+(1%start_time:~3,2%-100)*60+(1%start_time:~6,2%-100)"
set /a "end_secs=((1%end_time:~0,2%-100)*3600)+(1%end_time:~3,2%-100)*60+(1%end_time:~6,2%-100)"
set /a "elapsed=end_secs-start_secs"
echo 编译耗时: %elapsed% 秒