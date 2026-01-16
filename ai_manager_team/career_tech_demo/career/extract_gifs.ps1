# 동영상에서 13프레임 GIF 애니메이션 추출 스크립트
# 사용법: .\extract_gifs.ps1

$ErrorActionPreference = "Stop"
$videoDir = Join-Path $PSScriptRoot "동영상"
$thumbDir = Join-Path $PSScriptRoot "thumbnails"

function Assert-Tool
{
	param(
		[string]$ToolName
	)

	if (-not (Get-Command $ToolName -ErrorAction SilentlyContinue)) {
		Write-Host "[ERROR] $ToolName not found. Install ffmpeg (includes ffprobe) or add it to PATH." -ForegroundColor Red
		exit 1
	}
}

Assert-Tool "ffmpeg"
Assert-Tool "ffprobe"

$ffmpegCommonArgs = @("-hide_banner", "-loglevel", "error", "-nostdin", "-y")

# 썸네일 폴더 생성
if (-not (Test-Path $thumbDir)) {
    New-Item -ItemType Directory -Path $thumbDir | Out-Null
}

$videos = @(
    "2002 밀맨.wmv",
    "2004 마작.avi",
    "2005 펭고.wmv",
    "2006 요리조리.wmv",
    "2006 북로그.wmv",
    "2006 댄스.avi",
    "2007 별마루.wmv",
    "2007 농장.wmv",
    "2007 호버.avi",
    "2008 복싱.wmv",
    "2010 메탈슬러그pve.wmv",
    "2010 메탈슬러그pvp.wmv",
    "2010 메탈슬러그editor.wmv",
    "2011 볼츠앤블립.mp4",
    "2012 액션리딩.mp4",
    "2018 가디우스.mkv",
    "2022 MmoSync프로토타입.mp4",
    "2025 제노니아.mp4"
)

Write-Host "`nExtracting animated GIFs (13 frames each)...`n" -ForegroundColor Cyan

foreach ($video in $videos) {
	$baseName = [System.IO.Path]::GetFileNameWithoutExtension($video)
	$srcPath = Join-Path $videoDir $video
	$gifPath = Join-Path $thumbDir "$baseName.gif"

	try {
		# 이미 있으면 스킵
		if (Test-Path $gifPath) {
			Write-Host "[SKIP] $baseName.gif" -ForegroundColor Yellow
			continue
		}

		# 원본 파일 확인
		if (-not (Test-Path $srcPath)) {
			Write-Host "[ERROR] File not found: $video" -ForegroundColor Red
			continue
		}

		Write-Host "[EXTRACT] $video" -ForegroundColor Green

		# 동영상 길이 가져오기
		$durationOutput = (& ffprobe -v error -show_entries format=duration -of default=noprint_wrappers=1:nokey=1 "$srcPath" 2>$null).Trim()
		$duration = 0.0
		if (-not [double]::TryParse($durationOutput, [Globalization.NumberStyles]::Float, [Globalization.CultureInfo]::InvariantCulture, [ref]$duration)) {
			Write-Host "  -> Failed to parse duration: $durationOutput" -ForegroundColor Red
			continue
		}

		if ($duration -le 0) {
			Write-Host "  -> Failed to get duration" -ForegroundColor Red
			continue
		}

		# 임시 폴더 생성
		$tempDir = Join-Path $thumbDir "temp_$baseName"
		if (Test-Path $tempDir) {
			Remove-Item -Path $tempDir -Recurse -Force
		}
		New-Item -ItemType Directory -Path $tempDir | Out-Null

		# 14등분 지점에서 13프레임 추출 (시작과 끝 제외)
		$interval = $duration / 14

		for ($i = 1; $i -le 13; $i++) {
			$timestamp = $interval * $i
			$frameFile = Join-Path $tempDir ("frame_{0:D2}.png" -f $i)

			# 프레임 추출 (가로 480px로 축소)
			& ffmpeg @ffmpegCommonArgs -ss $timestamp -i "$srcPath" -vframes 1 -vf "scale=480:-1" "$frameFile" 2>$null | Out-Null
		}

		# 추출된 프레임 수 확인
		$frameCount = (Get-ChildItem -Path $tempDir -Filter "*.png").Count

		if ($frameCount -eq 0) {
			Write-Host "  -> No frames extracted" -ForegroundColor Red
			Remove-Item -Path $tempDir -Recurse -Force
			continue
		}

		# 프레임들을 GIF로 합치기 (1초에 2프레임 = 6.5초 애니메이션, 무한 반복)
		$framePattern = Join-Path $tempDir "frame_%02d.png"
		& ffmpeg @ffmpegCommonArgs -framerate 2 -i "$framePattern" -vf "split[s0][s1];[s0]palettegen=max_colors=128[p];[s1][p]paletteuse=dither=bayer" -loop 0 "$gifPath" 2>$null | Out-Null

		# 임시 폴더 삭제
		Remove-Item -Path $tempDir -Recurse -Force

		if (Test-Path $gifPath) {
			$size = (Get-Item $gifPath).Length / 1KB
			Write-Host ("  -> Success! ({0:N0} KB)" -f $size) -ForegroundColor Cyan
		} else {
			Write-Host "  -> Failed to create GIF" -ForegroundColor Red
		}
	}
	catch {
		Write-Host ("[ERROR] {0}: {1}" -f $video, $_.Exception.Message) -ForegroundColor Red
	}
}

Write-Host "`nDone! Check thumbnails folder.`n" -ForegroundColor Green
