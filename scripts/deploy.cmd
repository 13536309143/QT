@echo off
setlocal
call "%~dp0env.cmd"
if errorlevel 1 exit /b 1
set "COURSE_TARGET=%~1"
if not defined COURSE_TARGET set "COURSE_TARGET=01_hello"
call "%~dp0vs-env.cmd"
if errorlevel 1 exit /b 1
rem Restrict target name to existing executables from this course.
set "COURSE_KNOWN="
for %%T in (01_hello 02_counter 03_form 04_editor 05_json 06_model 07_paint 08_worker 09_http 10_sql 11_designer 12_resources 13_quick 14_controls 15_settings 16_tcp taskdesk) do if "%COURSE_TARGET%"=="%%T" set "COURSE_KNOWN=1"
if not defined COURSE_KNOWN exit /b 1
set "COURSE_EXE=%COURSE_ROOT%\build\Release\%COURSE_TARGET%.exe"
if "%COURSE_TARGET%"=="taskdesk" set "COURSE_EXE=%COURSE_ROOT%\build\projects\taskdesk\Release\taskdesk.exe"
if "%COURSE_TARGET%"=="13_quick" set "COURSE_EXE=%COURSE_ROOT%\build\examples\13_quick\Release\13_quick.exe"
if not exist "%COURSE_EXE%" exit /b 1
set "COURSE_DIST=%COURSE_ROOT%\dist\%COURSE_TARGET%"
if not exist "%COURSE_DIST%" mkdir "%COURSE_DIST%"
copy /y "%COURSE_EXE%" "%COURSE_DIST%\%COURSE_TARGET%.exe" >nul
if errorlevel 1 exit /b 1
"%QT_ROOT%\bin\windeployqt.exe" --release --compiler-runtime --qmldir "%COURSE_ROOT%\examples\13_quick" "%COURSE_DIST%\%COURSE_TARGET%.exe"
exit /b %errorlevel%
