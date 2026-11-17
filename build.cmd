@echo off
call "%~dp0scripts\build.cmd"
if errorlevel 1 (
  echo Build failed. Read the first error above.
  pause
  exit /b 1
)
echo Build and tests passed.
pause
