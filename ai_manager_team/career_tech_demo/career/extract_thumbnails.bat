@echo off
chcp 65001 >nul
cd /d "%~dp0"

if not exist "thumbnails" mkdir thumbnails

REM Preflight: ffmpeg must be installed or available in PATH.
where ffmpeg >nul 2>nul
if errorlevel 1 (
	echo [ERROR] ffmpeg not found. Install ffmpeg or add it to PATH. Example: winget install Gyan.FFmpeg
	goto :eof
)

echo Extracting thumbnails...

call :extract "2002 밀맨.wmv"
call :extract "2004 마작.avi"
call :extract "2005 펭고.wmv"
call :extract "2006 요리조리.wmv"
call :extract "2006 북로그.wmv"
call :extract "2006 댄스.avi"
call :extract "2007 별마루.wmv"
call :extract "2007 농장.wmv"
call :extract "2007 호버.avi"
call :extract "2008 복싱.wmv"
call :extract "2010 메탈슬러그pve.wmv"
call :extract "2010 메탈슬러그pvp.wmv"
call :extract "2010 메탈슬러그editor.wmv"
call :extract "2011 볼츠앤블립.mp4"
call :extract "2012 액션리딩.mp4"
call :extract "2018 가디우스.mkv"
call :extract "2022 mmosync prototype.mp4"
call :extract "2025 제노니아.mp4"

echo.
echo Done! Check thumbnails folder.
REM If you want "pause at end" behavior (double-click), pass: --pause
if /i "%~1"=="--pause" pause
goto :eof

:extract
set "fname=%~n1"
if exist "thumbnails\%fname%.png" (
    echo [SKIP] %fname%.png
    goto :eof
)
echo [EXTRACT] %~1
ffmpeg -hide_banner -loglevel error -nostdin -y -ss 5 -i "동영상\%~1" -vframes 1 -vf "scale=640:-1" -q:v 2 "thumbnails\%fname%.png" 2>nul
if not exist "thumbnails\%fname%.png" (
    ffmpeg -hide_banner -loglevel error -nostdin -y -ss 0 -i "동영상\%~1" -vframes 1 -vf "scale=640:-1" -q:v 2 "thumbnails\%fname%.png" 2>nul
)
goto :eof
