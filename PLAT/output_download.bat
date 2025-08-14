@echo off

set OUT_DIR=%BUILD_PRO_TARGET%_download
set bootloader_dir=openlib\%BUILD_PRO_TARGET%\bin
set project_dir=gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%

echo --------- Package dowmload ---------
echo BUILD_PRO_NAME=%BUILD_PRO_NAME%
echo BUILD_PRO_TARGET=%BUILD_PRO_TARGET%
echo PROJECT_NAME=%PROJECT_NAME%
echo project_dir=%project_dir%
echo bootloader_dir=%bootloader_dir%

if exist .\%OUT_DIR% (
    del /s /q %OUT_DIR% > nul
    rd  /s /q %OUT_DIR%
)
mkdir %OUT_DIR%

xcopy /y /q  %bootloader_dir%\ap_bootloader.bin                    %OUT_DIR%\
xcopy /y /q  %bootloader_dir%\ap_bootloader.elf                    %OUT_DIR%\
xcopy /y /q  %bootloader_dir%\ap_bootloader.map                    %OUT_DIR%\

copy  /y     %project_dir%\comdb.txt                               %OUT_DIR%\
copy  /y     %project_dir%\ap_%PROJECT_NAME%.bin                   %OUT_DIR%\
copy  /y     %project_dir%\ap_%PROJECT_NAME%.elf                   %OUT_DIR%\
copy  /y     %project_dir%\ap_%PROJECT_NAME%.map                   %OUT_DIR%\
copy  /y     %project_dir%\%PROJECT_NAME%.binpkg                   %OUT_DIR%\%BUILD_PRO_NAME%.binpkg

xcopy /y /q  .\mbtk\releasepack\%BUILD_PRO_NAME%\config_*.ini      %OUT_DIR%\
xcopy /y /q  .\mbtk\releasepack\%BUILD_PRO_NAME%\format_*.json     %OUT_DIR%\
xcopy /y /q  .\mbtk\releasepack\%BUILD_PRO_NAME%\*.bin             %OUT_DIR%\

echo ---------- Package end ------------