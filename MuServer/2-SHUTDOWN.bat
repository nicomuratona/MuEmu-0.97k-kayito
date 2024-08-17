@echo off
title Kayito MuServer ShutDown
color A

echo Closing MuServer Executables. . .

set servers=ConnectServer JoinServer DataServer GameServer

for %%s in (%servers%) do (
    echo.
    taskkill /F /IM %%s.exe
    timeout 2 > nul
)

cls
exit
