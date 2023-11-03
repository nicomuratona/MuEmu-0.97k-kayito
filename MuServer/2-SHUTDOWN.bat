@Echo Off
Title Kayito ShutDown Server
color A

taskkill /F /IM ConnectServer.exe
timeout 1 > nul
taskkill /F /IM JoinServer.exe
timeout 1 > nul
taskkill /F /IM DataServer.exe
timeout 1 > nul
taskkill /F /IM GameServer.exe
timeout 1 > nul

cls
exit