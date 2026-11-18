$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path $PSScriptRoot -Parent
$qtPrefix = if ($env:QT_ROOT) { $env:QT_ROOT } else { Join-Path $projectRoot 'sdk/6.8.3/msvc2022_64' }
$env:PATH = (Join-Path $qtPrefix 'bin') + ';' + $env:PATH
$env:QT_PLUGIN_PATH = Join-Path $qtPrefix 'plugins'
$env:QT_QPA_PLATFORM_PLUGIN_PATH = Join-Path $qtPrefix 'plugins/platforms'
# Use a real desktop platform, not the ordinary offscreen smoke-check platform.
$env:QT_QPA_PLATFORM = 'windows'
foreach ($target in '17_opengl','18_vulkan') {
    $exe = Join-Path $projectRoot "build/Release/$target.exe"
    if (!(Test-Path -LiteralPath $exe)) { throw "$target not built. Run scripts/build-graphics.cmd first." }
    # Interactive renderers must expose their windows; each self-test closes automatically.
    $process = Start-Process -FilePath $exe -ArgumentList '--self-test' -WindowStyle Normal -PassThru
    if (!$process.WaitForExit(20000)) { $process.Kill(); throw "$target graphics check timed out" }
    if ($process.ExitCode -ne 0) { throw "$target graphics check failed: $($process.ExitCode)" }
    Write-Host "PASS graphics $target"
}
