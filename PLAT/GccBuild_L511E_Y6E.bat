@echo off && setlocal enabledelayedexpansion
@echo %PATH% | findstr /c:"%~dp0tools/msys64/usr/bin">nul
@if %errorlevel% equ 1 set PATH=%~dp0tools/msys64/usr/bin;%PATH%
rem @set PATH=%~dp0tools/msys64/usr/bin;%PATH%
cls
set PROJECT_NAME=app_demo
set BOARD_NAME=ec7xx_0h00
set CHIP_NAME=ec7xx
set CHIP_TYPE=ec716e
set CORE_NAME=ap
set EUTRAN_MODE=cat_mode
set BUILD_OPTION=merge
set UNILOG=false
set TOOLCHAIN_NAME=GCC
set CUST=common
set SDKREL=false
set OPENCPU=true
set LTO_ENABLE=false
set GCF_ENABLE=false
set RAM_ENBALE=true
set ROM_ENBALE=false
set LESS_LOG=false
set PWR_TEST=false
set BUILD_HEADBIN=false
set GCCLIB_PATH="C:\gcc"
set COMDBLIB_PATH=".\prebuild\PLAT\lib\gcc\%CHIP_TYPE%\ram"
set CPBIN_SUBPATH=ram
set PKG_MAPDEF=pkg_716e_mapdef
set PKG_PRODUCT="EC716E_PRD"
set OUTPUT_NAME=%BOARD_NAME%_%CHIP_TYPE%
set JOBNUMBER=j8

echo build.bat version 20180330
echo GCCLIB_PATH: %GCCLIB_PATH%


if not %GCCLIB_PATH% == "" (
   if not exist %GCCLIB_PATH% (
      echo ERROR: Please check GCCLIB_PATH setting, exit!!!
      goto end
   )
) else (
	echo ERROR: Please set GCCLIB_PATH firstly, exit!!!
	goto end
)

@rem -------------------------------------------------------
@rem mbtk opencpu platform config
@rem -------------------------------------------------------
set BUILD_PRO_BOARD=EC716E
set BUILD_PRO_CHIP=L511C_Y6
set BUILD_PRO_NAME=L511E_Y6E
set BUILD_PRO_TARGET=L511E_Y6E


@rem mbtk add for copy comdb_cust_lib to tools
xcopy /f /y .\openlib\%BUILD_PRO_TARGET%\comdb_cust_lib.txt  .\tools\



@rem set build_config.inc file
type nul >.\middleware\developed\build_config.inc
echo MBTK_PRODUCT_BOARD=%BUILD_PRO_BOARD%>.\middleware\developed\build_config.inc
echo MBTK_PRODUCT_CHIP=%BUILD_PRO_CHIP%>>.\middleware\developed\build_config.inc
echo MBTK_PRODUCT_NAME=%BUILD_PRO_NAME%>>.\middleware\developed\build_config.inc

@rem set build factory feature
if "%1" == "-f" (
    echo CFLAGS += -DFACTORY_TEST_MODE>>.\middleware\developed\build_config.inc
)
rem duplicate comdblib.txt from 'COMDBLIB_PATH' to '.\tools\'
if exist %COMDBLIB_PATH%\comdblib.txt (
	xcopy /f /y %COMDBLIB_PATH%\comdblib.txt .\tools\
)


rem --------- if more board/project add here-------------
rem --------- board/project parsing end------------------



echo PARAMETERS: %PARAMETERS% | findstr "clall"
if not errorlevel 1 (
	make.exe -%JOBNUMBER%  clean-gccall TYPE=%CHIP_TYPE% TARGET=%BOARD_NAME% PROJECT=%PROJECT_NAME% CORE=%CORE_NAME% 
	echo clean all done ok...
	goto end
)

echo PARAMETERS: %PARAMETERS% | findstr "clean"
if not errorlevel 1 (
	make.exe -%JOBNUMBER%  clean-gcc TYPE=%CHIP_TYPE% TARGET=%BOARD_NAME% PROJECT=%PROJECT_NAME% CORE=%CORE_NAME%
	echo clean done ok...
	goto end
)

set starttime=%time%
echo Start time: %date% %starttime%

if "%BUILD_OPTION%"=="merge" (
	if exist .\tools\comdblib.txt (
		echo start logprepass
		
		
		make.exe -%JOBNUMBER% build-unilog TYPE=%CHIP_TYPE% TARGET=%BOARD_NAME% PROJECT=%PROJECT_NAME% CORE=%CORE_NAME% BUILD_UNILOG=true BUILD_CUST=%CUST% SDK=true
		
		
		rem this header file will compile with device code
		copy .\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\debug_log_ap.h .\middleware\developed\debug\inc\ || (goto:failHandle)
		if exist .\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\dbversion.h (
		copy .\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\dbversion.h .\middleware\developed\debug\inc\ || (goto:failHandle)
		)
		
	    echo > .failed.tmp
	   (make.exe -%JOBNUMBER% gccall TYPE=%CHIP_TYPE% TARGET=%BOARD_NAME% V=%VERBOSE% EUTRAN_MODE=%EUTRAN_MODE% PROJECT=%PROJECT_NAME% CORE=ap BUILD_UNILOG=true BUILD_CUST=%CUST% SDK=true 2>&1 && del .failed.tmp) | tee.exe .\gccout\%OUTPUT_NAME%\ap\outbuildlog.txt

	    if exist .failed.tmp (
	    del .failed.tmp
	    goto failHandle
	    )
		
		.\tools\fcelf.exe -T -bin .\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\ap_%PROJECT_NAME%.bin -size AP_PKGIMG_LIMIT_SIZE ^
								-bin .\prebuild\FW\lib\gcc\%CHIP_TYPE%\%CPBIN_SUBPATH%\cp-demo-flash.bin -size CP_PKGIMG_LIMIT_SIZE ^
								-bin .\openlib\%BUILD_PRO_TARGET%\bin\ap_bootloader.bin -size BOOTLOADER_PKGIMG_LIMIT_SIZE ^
								-h .\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\mem_map.txt
								
		if not errorlevel 0 (
			goto failHandle
		)
	    .\tools\fcelf.exe -M  -input .\openlib\%BUILD_PRO_TARGET%\bin\ap_bootloader.bin -addrname BL_PKGIMG_LNA -flashsize BOOTLOADER_PKGIMG_LIMIT_SIZE  ^
		-input .\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\ap_%PROJECT_NAME%.bin -addrname AP_PKGIMG_LNA -flashsize AP_PKGIMG_LIMIT_SIZE ^
		-input .\prebuild\FW\lib\gcc\%CHIP_TYPE%\%CPBIN_SUBPATH%\cp-demo-flash.bin -addrname CP_PKGIMG_LNA -flashsize CP_PKGIMG_LIMIT_SIZE ^
		-input .\mbtk\releasepack\%BUILD_PRO_NAME%\%CHIP_TYPE%_PrMgrCfg.json.bin -addrname XPKGDPRM_VIRTIMG_MERGE_LNA -flashsize XPKG_VIRTIMG_LOAD_SIZE ^
		-input .\mbtk\releasepack\%BUILD_PRO_NAME%\format_%CHIP_TYPE%.json.bin -addrname XPKGDCMN_VIRTIMG_MERGE_LNA -flashsize XPKG_VIRTIMG_LOAD_SIZE ^
		-input .\mbtk\releasepack\%BUILD_PRO_NAME%\config_%CHIP_TYPE%_prd_uart.baseini.bin -addrname XPKGDCMN_VIRTIMG_MERGE_LNA -flashsize XPKG_VIRTIMG_LOAD_SIZE ^
		-input .\mbtk\releasepack\%BUILD_PRO_NAME%\config_%CHIP_TYPE%_prd_usb.baseini.bin -addrname XPKGDCMN_VIRTIMG_MERGE_LNA -flashsize XPKG_VIRTIMG_LOAD_SIZE ^
		-pkgmode 1  ^
		-banoldtool 1 ^
		-productname %PKG_PRODUCT% ^
		-def .\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\mem_map.txt ^
		-outfile .\gccout\%OUTPUT_NAME%\ap\%PROJECT_NAME%\%PROJECT_NAME%.binpkg
		if not errorlevel 0 (
			goto failHandle
		)


		 if exist .\tools\UpdateDBPattern.txt (
		 type .\tools\UpdateDBPattern.txt
	     )

		 goto complete

	)

)


:complete

set endtime=%time%
echo .
echo End time: %date% %endtime%

set /a h1=%starttime:~0,2%
set /a m1=1%starttime:~3,2%-100
set /a s1=1%starttime:~6,2%-100
set /a h2=%endtime:~0,2%
set /a m2=1%endtime:~3,2%-100
set /a s2=1%endtime:~6,2%-100
if %h2% LSS %h1% set /a h2=%h2%+24
set /a ts1=%h1%*3600+%m1%*60+%s1%
set /a ts2=%h2%*3600+%m2%*60+%s2%
set /a ts=%ts2%-%ts1%
set /a h=%ts%/3600
set /a m=(%ts%-%h%*3600)/60
set /a s=%ts%%%60
echo Built took %h% hours %m% minutes %s% seconds

echo     #######################################################################
echo     ##                                                                   ##
echo     ##                 ########     ###     ######   ######              ##
echo     ##                 ##     ##   ## ##   ##    ## ##    ##             ##
echo     ##                 ##     ##  ##   ##  ##       ##                   ##
echo     ##                 ########  ##     ##  ######   ######              ##
echo     ##                 ##        #########       ##       ##             ##
echo     ##                 ##        ##     ## ##    ## ##    ##             ##
echo     ##                 ##        ##     ##  ######   ######              ##
echo     ##                                                                   ##
echo     #######################################################################

echo build successfully

@rem --------------------------------------------------------------------
@rem mbtk add to copy download package
call output_download.bat

@rem --------------------------------------------------------------------

if %UNILOG%==true (
if exist .\tools\UpdateDBPattern.txt (
type .\tools\UpdateDBPattern.txt
)
)

:end
goto:eof

:failHandle
echo fail
echo     #######################################################################
echo     ##                                                                   ##
echo     ##                    ########    ###     ####  ##                   ##
echo     ##                    ##         ## ##     ##   ##                   ##
echo     ##                    ##        ##   ##    ##   ##                   ##
echo     ##                    ######   ##     ##   ##   ##                   ##
echo     ##                    ##       #########   ##   ##                   ##
echo     ##                    ##       ##     ##   ##   ##                   ##
echo     ##                    ##       ##     ##  ####  ########             ##
echo     ##                                                                   ##
echo     #######################################################################
exit /b 1
goto:eof

