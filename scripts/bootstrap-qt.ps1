[CmdletBinding()]
param()
$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path $PSScriptRoot -Parent
$config = Get-Content -LiteralPath (Join-Path $projectRoot 'qt-version.json') -Raw | ConvertFrom-Json
$toolsRoot = Join-Path $projectRoot '.tools'
$toolsPython = Join-Path $toolsRoot 'Scripts\python.exe'
$sdkRoot = Join-Path $projectRoot 'sdk'
$qtPrefix = Join-Path $sdkRoot ($config.qt + '/' + $config.sdkSubdir)
$marker = Join-Path $qtPrefix '.course-install-complete'
if (Test-Path -LiteralPath $marker) {
    Write-Host "Qt SDK already installed: $qtPrefix"
    exit 0
}
if (!(Test-Path -LiteralPath $toolsPython)) {
    python -m venv $toolsRoot
    if ($LASTEXITCODE -ne 0) { throw 'Python venv failed; install Python 3.10+.' }
}
& $toolsPython -m pip install ('aqtinstall==' + $config.aqtinstall)
if ($LASTEXITCODE -ne 0) { throw 'Failed to install aqtinstall.' }
$installArguments = @('-m','aqt','install-qt','windows','desktop',$config.qt,$config.architecture,'--outputdir',$sdkRoot,'--archives') + @($config.archives)
& $toolsPython @installArguments
if ($LASTEXITCODE -ne 0) { throw 'Qt download/install failed. Check network and retry.' }
if (!(Test-Path -LiteralPath (Join-Path $qtPrefix 'lib/cmake/Qt6/Qt6Config.cmake'))) { throw 'Qt CMake config missing.' }
if (!(Test-Path -LiteralPath (Join-Path $qtPrefix 'bin/Qt6Quick.dll'))) { throw 'Qt Quick missing.' }
New-Item -ItemType File -Path $marker -Force | Out-Null
Write-Host "Qt SDK ready: $qtPrefix"
