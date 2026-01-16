@echo off
setlocal enabledelayedexpansion
chcp 65001 >nul
cd /d "%~dp0"

if not exist "thumbnails" mkdir thumbnails

if "%~1"=="" (
	echo Usage: %~nx0 "video_filename"
	echo Example: %~nx0 "2004 마작.avi"
	goto :eof
)

REM Preflight: ffmpeg/ffprobe must be installed or available in PATH.
where ffmpeg >nul 2>nul
if errorlevel 1 (
	echo [ERROR] ffmpeg not found. Install ffmpeg or add it to PATH. Example: winget install Gyan.FFmpeg
	goto :eof
)

where ffprobe >nul 2>nul
if errorlevel 1 (
	echo [ERROR] ffprobe not found. Install ffmpeg which includes ffprobe, or add it to PATH.
	goto :eof
)

set "video=%~1"
set "fname=%~n1"

echo Processing: %fname%

REM Get duration
for /f "tokens=*" %%d in ('ffprobe -v error -show_entries format^=duration -of default^=noprint_wrappers^=1:nokey^=1 "동영상\%video%" 2^>nul') do set "dur=%%d"

REM Create temp folder
if exist "thumbnails\temp" rd /s /q "thumbnails\temp"
mkdir "thumbnails\temp"

REM Extract 13 frames using PowerShell for math
powershell -NoProfile -ExecutionPolicy Bypass -Command "$d=[double]'%dur%';$i=$d/14;1..13|%%{$t=$i*$_;ffmpeg -hide_banner -loglevel error -nostdin -y -ss $t -i '동영상\%video%' -vframes 1 -vf scale=480:-1 ('thumbnails\temp\f{0:D2}.png' -f $_) 2>$null}"

REM Create GIF
ffmpeg -hide_banner -loglevel error -nostdin -y -framerate 2 -i "thumbnails\temp\f%%02d.png" -filter_complex "[0:v]split[a][b];[a]palettegen=max_colors=128[p];[b][p]paletteuse=dither=bayer" -loop 0 "thumbnails\%fname%.gif" 2>nul

REM Cleanup
rd /s /q "thumbnails\temp" 2>nul

if exist "thumbnails\%fname%.gif" (
    echo Done: %fname%.gif
) else (
    echo Failed: %fname%
)
