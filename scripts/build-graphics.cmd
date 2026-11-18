@echo off
setlocal
call "%~dp0env.cmd"
if errorlevel 1 exit /b 1
call "%~dp0vs-env.cmd"
if errorlevel 1 exit /b 1
cmake -S "%COURSE_ROOT%" -B "%COURSE_ROOT%\build" -G "Visual Studio 17 2022" -A x64 "-DCMAKE_PREFIX_PATH=%QT_ROOT%" -DCOURSE_BUILD_OPENGL=ON -DCOURSE_BUILD_VULKAN=ON
if errorlevel 1 exit /b 1
cmake --build "%COURSE_ROOT%\build" --config Release --target 17_opengl 18_vulkan --parallel 2
exit /b %errorlevel%
