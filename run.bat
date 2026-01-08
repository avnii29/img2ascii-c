@echo off
if "%~1"=="" (
    img2ascii.exe assets\sample.jpg
) else (
    img2ascii.exe %1
)
