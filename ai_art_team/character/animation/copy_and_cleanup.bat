@echo off
chcp 65001 > nul
echo === 캐릭터 이미지 정리 및 복사 스크립트 ===
echo.

set SOURCE_DIR=%~dp0
set TARGET_DIR=%~dp0..\..\..\resource\client\character\character_images

echo 1. 백업 파일 삭제 중...
del /Q "%SOURCE_DIR%*_backup.png" 2>nul
echo    완료!
echo.

echo 2. PNG 파일 복사 중...
echo    원본: %SOURCE_DIR%
echo    대상: %TARGET_DIR%
echo.

copy /Y "%SOURCE_DIR%archer.png" "%TARGET_DIR%\"
copy /Y "%SOURCE_DIR%assassin.png" "%TARGET_DIR%\"
copy /Y "%SOURCE_DIR%bat.png" "%TARGET_DIR%\"
copy /Y "%SOURCE_DIR%bear.png" "%TARGET_DIR%\"
copy /Y "%SOURCE_DIR%cupid.png" "%TARGET_DIR%\"
copy /Y "%SOURCE_DIR%dragon.png" "%TARGET_DIR%\"
copy /Y "%SOURCE_DIR%golem.png" "%TARGET_DIR%\"
copy /Y "%SOURCE_DIR%hades.png" "%TARGET_DIR%\"
copy /Y "%SOURCE_DIR%phoenix.png" "%TARGET_DIR%\"
copy /Y "%SOURCE_DIR%spider.png" "%TARGET_DIR%\"
copy /Y "%SOURCE_DIR%warrior.png" "%TARGET_DIR%\"
copy /Y "%SOURCE_DIR%zeus.png" "%TARGET_DIR%\"

echo.
echo === 완료! ===
pause
