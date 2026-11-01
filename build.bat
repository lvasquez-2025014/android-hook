@echo off
setlocal
cd /d %~dp0

if exist "C:\Program Files\Android\Android Studio\jbr" (
    set "JAVA_HOME=C:\Program Files\Android\Android Studio\jbr"
) else if exist "C:\Program Files\Android\Android Studio\jre" (
    set "JAVA_HOME=C:\Program Files\Android\Android Studio\jre"
) else if exist "C:\Program Files\Microsoft\jdk-11.0.32.101-hotspot" (
    set "JAVA_HOME=C:\Program Files\Microsoft\jdk-11.0.32.101-hotspot"
)
if defined JAVA_HOME set "PATH=%JAVA_HOME%\bin;%PATH%"

title AIMKILL APK Builder

:menu
cls
echo ========================================
echo       AIMKILL APK BUILD SYSTEM
echo ========================================
echo.
echo [1] Build Debug APK (Standard)
echo [2] Build Release APK (Optimized)
echo [3] Clean Project (Fix Errors)
echo [4] Exit
echo.
set /p choice="Enter choice (1-4): "

if "%choice%"=="1" goto debug
if "%choice%"=="2" goto release
if "%choice%"=="3" goto clean
if "%choice%"=="4" goto end

:debug
echo.
echo [*] Building Debug APK...
call gradlew.bat assembleDebug
if %ERRORLEVEL% equ 0 (
    echo.
    echo [!] Build SUCCESSFUL!
    echo [*] Opening output folder...
    start "" "app\build\outputs\apk\debug\"
) else (
    echo.
    echo [X] Build FAILED! Please check the logs above.
)
pause
goto menu

:release
echo.
echo [*] Building Release APK...
call gradlew.bat assembleRelease
if %ERRORLEVEL% equ 0 (
    echo.
    echo [!] Build SUCCESSFUL!
    echo [*] Opening output folder...
    start "" "app\build\outputs\apk\release\"
) else (
    echo.
    echo [X] Build FAILED! Please check the logs above.
)
pause
goto menu

:clean
echo.
echo [*] Cleaning project...
call gradlew.bat clean
echo.
echo [!] Clean complete.
pause
goto menu

:end
exit
