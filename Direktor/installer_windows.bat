@echo off

TMPFILE=tmp\install.ps1
if not exist tmp mkdir tmp
if exist "%TMPFILE%" del /Y "%TMPFILE%"
powershell -command Invoke-WebRequest -Uri "https://github.com/Amyseni/organik/releases/latest/download/installer_powershell.ps1" -OutFile "tmp\install.ps1" && powershell -ExecutionPolicy "RemoteSigned" -File "tmp\install.ps1" && pause