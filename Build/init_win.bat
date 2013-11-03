@echo off
set _mm7_install_dir=C:\Program Files\Buka\MMCollection\MM_VII



REM Check Windows Version

ver | findstr /i "5\.0\." > nul
IF %ERRORLEVEL% EQU 0 goto win_2000

ver | findstr /i "5\.1\." > nul
IF %ERRORLEVEL% EQU 0 goto win_xp

ver | findstr /i "5\.2\." > nul
IF %ERRORLEVEL% EQU 0 goto win_2003

ver | findstr /i "6\.0\." > nul
IF %ERRORLEVEL% EQU 0 goto win_vista

ver | findstr /i "6\.1\." > nul
IF %ERRORLEVEL% EQU 0 goto win_7

echo Machine OS cannot be determined.
goto end

:win_7
echo Win 7
reg add HKCU\Environment /f /v "WoMM_MM7_INSTALL_DIR" /t REG_SZ /d "%_mm7_install_dir%"

reg query HKCU\Environment /v "WoMM_MM7_INSTALL_DIR"
goto end

:win_2000
echo Win 2000
goto end

:win_xp
echo Win XP
reg add "HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Environment" /f /v "WoMM_MM7_INSTALL_DIR" /t REG_SZ /d "%_mm7_install_dir%"


reg query "HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Environment" /v "WoMM_MM7_INSTALL_DIR"
goto end

:win_vista
echo Win Vista
goto end

:win_2003
echo Win 2003
goto end

:check_vars
goto end

:end
pause