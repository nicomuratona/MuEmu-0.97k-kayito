@echo off
title Kayito MuServer Startup
color A

start /min /d ".\ConnectServer" ConnectServer.exe
timeout 1 > nul

start /min /d ".\JoinServer" JoinServer.exe
timeout 1 > nul

start /min /d ".\DataServer" DataServer.exe
timeout 1 > nul

start /d ".\GameServer" GameServer.exe
timeout 1 > nul

echo.
echo.Se ejecuto correctamente!
timeout 1 > nul

cls
exit