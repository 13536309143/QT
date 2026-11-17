@echo off
rem Called by build/run/deploy; changes are local to caller's setlocal.
for %%I in ("%~dp0..") do set "COURSE_ROOT=%%~fI"
if not defined QT_ROOT set "QT_ROOT=%COURSE_ROOT%\sdk\6.8.3\msvc2022_64"
if not exist "%QT_ROOT%\lib\cmake\Qt6\Qt6Config.cmake" (
  echo Qt 6 SDK not found. Run scripts\bootstrap-qt.ps1 first.
  exit /b 1
)
set "PATH=%QT_ROOT%\bin;%PATH%"
set "QT_PLUGIN_PATH=%QT_ROOT%\plugins"
set "QT_QPA_PLATFORM_PLUGIN_PATH=%QT_ROOT%\plugins\platforms"
set "QML_IMPORT_PATH=%QT_ROOT%\qml"
set "QML2_IMPORT_PATH=%QT_ROOT%\qml"
exit /b 0
