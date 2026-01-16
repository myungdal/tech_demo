# 동영상에서 썸네일 이미지 추출 스크립트
# 사용법: .\extract_thumbnails.ps1
# 필수: ffmpeg가 설치되어 있어야 함 (winget install ffmpeg)

$ErrorActionPreference = "Stop"
$videoDir = Join-Path $PSScriptRoot "동영상"
$thumbDir = Join-Path $PSScriptRoot "thumbnails"

if (-not (Get-Command "ffmpeg" -ErrorAction SilentlyContinue)) {
	Write-Host "[ERROR] ffmpeg not found. Install ffmpeg or add it to PATH. (ex: winget install Gyan.FFmpeg)" -ForegroundColor Red
	exit 1
}

$ffmpegCommonArgs = @("-hide_banner", "-loglevel", "error", "-nostdin", "-y")

# 썸네일 폴더 생성
if (-not (Test-Path $thumbDir)) {
    New-Item -ItemType Directory -Path $thumbDir | Out-Null
    Write-Host "썸네일 폴더 생성: $thumbDir"
}

# 동영상 확장자 목록
$videoExtensions = @(".mp4", ".avi", ".wmv", ".mkv", ".mov", ".webm")

# 동영상 파일 목록 가져오기
$videoFiles = Get-ChildItem -Path $videoDir -File | Where-Object {
    $videoExtensions -contains $_.Extension.ToLower()
}

Write-Host "`n총 $($videoFiles.Count)개의 동영상 파일 발견`n"

foreach ($video in $videoFiles) {
    $baseName = [System.IO.Path]::GetFileNameWithoutExtension($video.Name)
    $thumbPath = Join-Path $thumbDir "$baseName.png"
    
    # 이미 썸네일이 있으면 스킵
    if (Test-Path $thumbPath) {
        Write-Host "[스킵] 이미 존재: $baseName.png"
        continue
    }
    
    Write-Host "[추출중] $($video.Name) -> $baseName.png"
    
	try {
		# 동영상 5초 지점에서 썸네일 추출 (5초 미만이면 0초)
		# 해상도: 원본 비율 유지하며 가로 640px로 조정
		$ffmpegArgs = @($ffmpegCommonArgs + @(
			"-ss", "5",
			"-i", $video.FullName,
			"-vframes", "1",
			"-vf", "scale=640:-1",
			"-q:v", "2",
			$thumbPath
		))

		& ffmpeg @ffmpegArgs 2>$null | Out-Null

		if (Test-Path $thumbPath) {
			Write-Host "  -> 성공!"
			continue
		}

		# 5초가 너무 길면 0초로 재시도
		Write-Host "  -> 5초 실패, 0초로 재시도..."
		$ffmpegArgs = @($ffmpegCommonArgs + @(
			"-ss", "0",
			"-i", $video.FullName,
			"-vframes", "1",
			"-vf", "scale=640:-1",
			"-q:v", "2",
			$thumbPath
		))
		& ffmpeg @ffmpegArgs 2>$null | Out-Null

		if (Test-Path $thumbPath) {
			Write-Host "  -> 성공!"
		} else {
			Write-Host "  -> 실패!" -ForegroundColor Red
		}
	}
	catch {
		Write-Host ("  -> 실패! {0}" -f $_.Exception.Message) -ForegroundColor Red
	}
}

Write-Host "`n완료! 썸네일 폴더: $thumbDir"
Write-Host "`n기존 이미지 파일도 썸네일 폴더로 복사하려면:"
Write-Host "  Copy-Item `"$videoDir\*.png`" `"$thumbDir\`""
Write-Host "  Copy-Item `"$videoDir\*.jpg`" `"$thumbDir\`""
