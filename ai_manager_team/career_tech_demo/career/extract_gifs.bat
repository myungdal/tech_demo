@echo off
chcp 65001 >nul
cd /d "%~dp0"

if not exist "thumbnails" mkdir thumbnails

REM Preflight: ffmpeg/ffprobe must be installed or available in PATH.
where ffmpeg >nul 2>nul
if errorlevel 1 (
	echo [ERROR] ffmpeg not found. Install ffmpeg or add it to PATH. Example: winget install Gyan.FFmpeg
	echo         Also requires ffprobe which comes with ffmpeg.
	goto :eof
)

where ffprobe >nul 2>nul
if errorlevel 1 (
	echo [ERROR] ffprobe not found. Install ffmpeg which includes ffprobe, or add it to PATH.
	goto :eof
)

echo Extracting animated GIFs (13 frames each)...
echo.

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
call :extract "2022 MmoSync프로토타입.mp4"
call :extract "2025 제노니아.mp4"

echo.
echo Done! Check thumbnails folder for GIF files.
if /i "%~1"=="--pause" pause
goto :eof

:extract
set "fname=%~n1"
set "src=동영상\%~1"
set "dst=thumbnails\%fname%.gif"

if exist "%dst%" (
    echo [SKIP] %fname%.gif
    goto :eof
)

echo [EXTRACT] %~1

REM 동영상 길이 가져오기
for /f "tokens=*" %%a in ('ffprobe -v error -show_entries format^=duration -of default^=noprint_wrappers^=1:nokey^=1 "%src%" 2^>nul') do set "duration=%%a"

REM 임시 폴더 생성
if not exist "thumbnails\temp_%fname%" mkdir "thumbnails\temp_%fname%"

REM PowerShell로 14등분 시간 계산하고 13프레임 추출
powershell -NoProfile -Command "$dur=[double]'%duration%';$interval=$dur/14;for($i=1;$i-le13;$i++){$t=$interval*$i;$f='thumbnails\temp_%fname%\frame_{0:D2}.png'-f$i;ffmpeg -hide_banner -loglevel error -nostdin -y -ss $t -i '%src%' -vframes 1 -vf scale=480:-1 $f 2>$null}"

REM 프레임들을 GIF로 합치기 (1초당 2프레임 = 3.5초 애니메이션)
ffmpeg -y -framerate 2 -i "thumbnails\temp_%fname%\frame_%%02d.png" -vf "split[s0][s1];[s0]palettegen=max_colors=128[p];[s1][p]paletteuse=dither=bayer" -loop 0 "%dst%" 2>nul

REM 임시 파일 정리
rd /s /q "thumbnails\temp_%fname%" 2>nul

if exist "%dst%" (
    echo   -^> Success!
) else (
    echo   -^> Failed!
)
goto :eof
