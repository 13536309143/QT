$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path $PSScriptRoot -Parent
$qtPrefix = if ($env:QT_ROOT) { $env:QT_ROOT } else { Join-Path $projectRoot 'sdk/6.8.3/msvc2022_64' }
$env:PATH = (Join-Path $qtPrefix 'bin') + ';' + $env:PATH
$env:QT_PLUGIN_PATH = Join-Path $qtPrefix 'plugins'
$env:QT_QPA_PLATFORM_PLUGIN_PATH = Join-Path $qtPrefix 'plugins/platforms'
$env:QML_IMPORT_PATH = Join-Path $qtPrefix 'qml'
$env:QML2_IMPORT_PATH = $env:QML_IMPORT_PATH
$env:QT_QPA_PLATFORM = 'offscreen'
$targets = '01_hello','02_counter','03_form','04_editor','05_json','06_model','07_paint','08_worker','09_http','10_sql','11_designer','12_resources'
foreach ($target in $targets) {
    $exePath = Join-Path $projectRoot "build/Release/$target.exe"
    $process = Start-Process -FilePath $exePath -ArgumentList '--smoke' -PassThru -WindowStyle Hidden
    if (!$process.WaitForExit(15000)) { $process.Kill(); throw "$target timed out" }
    if ($process.ExitCode -ne 0) { throw "$target failed: $($process.ExitCode)" }
    Write-Host "PASS $target"
}
foreach ($relative in 'projects/taskdesk/Release/taskdesk.exe','examples/13_quick/Release/13_quick.exe') {
    $process = Start-Process -FilePath (Join-Path $projectRoot "build/$relative") -ArgumentList '--smoke' -PassThru -WindowStyle Hidden
    if (!$process.WaitForExit(15000)) { $process.Kill(); throw "$relative timed out" }
    if ($process.ExitCode -ne 0) { throw "$relative failed: $($process.ExitCode)" }
    Write-Host "PASS $relative"
}
