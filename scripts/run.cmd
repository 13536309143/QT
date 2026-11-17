@echo off
setlocal
call "%~dp0env.cmd"
if errorlevel 1 exit /b 1
set "COURSE_TARGET=%~1"
if not defined COURSE_TARGET set "COURSE_TARGET=04_editor"
set "COURSE_EXE=%COURSE_ROOT%\build\Release\%COURSE_TARGET%.exe"
if "%COURSE_TARGET%"=="taskdesk" set "COURSE_EXE=%COURSE_ROOT%\build\projects\taskdesk\Release\taskdesk.exe"
if "%COURSE_TARGET%"=="13_quick" set "COURSE_EXE=%COURSE_ROOT%\build\examples\13_quick\Release\13_quick.exe"
if not exist "%COURSE_EXE%" (
  echo Example not built. Run scripts\build.cmd first.
  exit /b 1
)
"%COURSE_EXE%" %2 %3 %4
exit /b %errorlevel%
