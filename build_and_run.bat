@echo off
echo ========================================
echo   Hotel Simulator - Build and Run
echo ========================================
echo.

:: Change to the directory where the batch file is located
cd /d "%~dp0"

:: Add MSYS2 MinGW to PATH (common installation path)
set PATH=%PATH%;C:\msys64\ucrt64\bin;C:\msys64\mingw64\bin

:: Check if GCC is available
where gcc >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] GCC not found!
    echo Please install MinGW or MSYS2.
    pause
    exit /b 1
)

echo [1/2] Compiling with GCC...
gcc -o hotel_simulator.exe src/main.c src/database.c src/utils.c src/player.c src/game.c src/chatbot.c src/admin.c -Iinclude -Wall
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Compilation failed!
    pause
    exit /b 1
)

echo.
echo ========================================
echo   Build successful! Starting game...
echo ========================================
echo.

:: Run the executable
hotel_simulator.exe

pause
