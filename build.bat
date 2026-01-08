@echo off
echo Building img2ascii...
gcc -Wall -Wextra -Wpedantic -Iinclude src/main.c src/image.c src/ascii.c src/edges.c -o img2ascii.exe -lm

if %errorlevel% equ 0 (
    echo.
    echo ========================================
    echo Build successful!
    echo ========================================
    echo.
    echo Usage:
    echo   .\img2ascii.exe                          - Default sample image
    echo   .\img2ascii.exe image.jpg                - Custom image
    echo   .\img2ascii.exe -w 100 image.jpg         - Set width
    echo   .\img2ascii.exe --mode hybrid image.jpg  - Choose mode
    echo.
    echo Modes: brightness, edges, hybrid ^(default^)
    echo.
) else (
    echo Build failed!
    exit /b 1
)
