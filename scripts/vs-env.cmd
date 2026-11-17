@echo off
rem Caller owns setlocal; keep the detected toolchain in that scope.
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" (
  echo Install Visual Studio 2022 with Desktop development with C++.
  exit /b 1
)
if not exist "%COURSE_ROOT%\build" mkdir "%COURSE_ROOT%\build"
"%VSWHERE%" -latest -version "[17.0,18.0)" -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath > "%COURSE_ROOT%\build\.vs-installation.txt"
if errorlevel 1 exit /b 1
set "COURSE_VS="
set /p "COURSE_VS=" < "%COURSE_ROOT%\build\.vs-installation.txt"
if not defined COURSE_VS (
  echo Visual Studio 2022 C++ x64 toolset not found.
  exit /b 1
)
call "%COURSE_VS%\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64
exit /b %errorlevel%
