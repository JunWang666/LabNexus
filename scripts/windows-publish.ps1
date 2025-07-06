param(
    [Parameter(Mandatory = $true)]
    [string]$ArchiveName,

    [Parameter(Mandatory = $true)]
    [string]$ExePath
)

# 设置错误处理
$ErrorActionPreference = "Stop"

Write-Host "开始打包 Qt 应用..." -ForegroundColor Green
Write-Host "归档名称: $ArchiveName" -ForegroundColor Yellow
Write-Host "可执行文件路径: $ExePath" -ForegroundColor Yellow

# 检查可执行文件是否存在
if (-not (Test-Path $ExePath))
{
    Write-Error "可执行文件不存在: $ExePath"
    exit 1
}

# 获取可执行文件名
$ExeFileName = Split-Path $ExePath -Leaf
Write-Host "可执行文件名: $ExeFileName" -ForegroundColor Yellow

# 创建打包目录
$PackageDir = ".\$ArchiveName"
if (Test-Path $PackageDir)
{
    Write-Host "删除已存在的打包目录: $PackageDir" -ForegroundColor Yellow
    Remove-Item $PackageDir -Recurse -Force
}

Write-Host "创建打包目录: $PackageDir" -ForegroundColor Yellow
New-Item -ItemType Directory -Path $PackageDir -Force | Out-Null

# 复制可执行文件
Write-Host "复制可执行文件到打包目录..." -ForegroundColor Yellow
Copy-Item $ExePath $PackageDir

# 使用 windeployqt 部署 Qt 依赖
Write-Host "使用 windeployqt 部署 Qt 依赖..." -ForegroundColor Yellow

# 检查 windeployqt 是否可用
$windeployqt = Get-Command "windeployqt.exe" -ErrorAction SilentlyContinue
if (-not $windeployqt)
{
    Write-Error "找不到 windeployqt.exe，请确保 Qt 的 bin 目录在 PATH 中"
    exit 1
}

# 运行 windeployqt
$targetExe = Join-Path $PackageDir $ExeFileName
try
{
    & windeployqt.exe --release $targetExe
    if ($LASTEXITCODE -ne 0)
    {
        Write-Error "windeployqt 执行失败，退出代码: $LASTEXITCODE"
        exit 1
    }
    Write-Host "Qt 依赖部署完成" -ForegroundColor Green
}
catch
{
    Write-Error "运行 windeployqt 时发生错误: $_"
    exit 1
}

# 复制可能需要的额外 DLL（如果存在）
$BuildDir = Split-Path $ExePath -Parent
$CommonDlls = @(
    "msvcp*.dll",
    "vcruntime*.dll",
    "concrt*.dll"
)

foreach ($dll in $CommonDlls)
{
    $dllFiles = Get-ChildItem -Path $BuildDir -Name $dll -ErrorAction SilentlyContinue
    foreach ($dllFile in $dllFiles)
    {
        $sourcePath = Join-Path $BuildDir $dllFile
        if (Test-Path $sourcePath)
        {
            Write-Host "复制额外 DLL: $dllFile" -ForegroundColor Yellow
            Copy-Item $sourcePath $PackageDir
        }
    }
}

# 显示打包目录内容
Write-Host "打包目录内容:" -ForegroundColor Yellow
Get-ChildItem $PackageDir | ForEach-Object { Write-Host "  $( $_.Name )" -ForegroundColor Cyan }

# 创建压缩包
$ZipPath = "$ArchiveName.zip"
if (Test-Path $ZipPath)
{
    Write-Host "删除已存在的压缩包: $ZipPath" -ForegroundColor Yellow
    Remove-Item $ZipPath -Force
}

Write-Host "创建压缩包: $ZipPath" -ForegroundColor Yellow
try
{
    Compress-Archive -Path $PackageDir -DestinationPath $ZipPath -CompressionLevel Optimal
    Write-Host "压缩包创建成功" -ForegroundColor Green
}
catch
{
    Write-Error "创建压缩包时发生错误: $_"
    exit 1
}

# 显示最终结果
$ZipSize = (Get-Item $ZipPath).Length
$ZipSizeMB = [math]::Round($ZipSize / 1MB, 2)
Write-Host "打包完成!" -ForegroundColor Green
Write-Host "压缩包大小: $ZipSizeMB MB" -ForegroundColor Green
Write-Host "输出文件: $ZipPath" -ForegroundColor Green

# 设置 GitHub Actions 输出
if ($env:GITHUB_ACTIONS -eq "true")
{
    echo "packageName=$ArchiveName" >> $env:GITHUB_OUTPUT
    Write-Host "GitHub Actions 输出已设置" -ForegroundColor Green
}
