@echo off
title Kayito MuServer Startup
color A

echo Starting MuServer Executables. . .

set servers=ConnectServer JoinServer DataServer GameServer

for %%s in (%servers%) do (
    echo.
    echo Starting %%s. . .
    start /min /d ".\%%s" %%s.exe
    timeout 2 > nul
)

cls
exit
