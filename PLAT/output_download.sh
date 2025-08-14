#!/bin/bash

echo --------- Package dowmload ---------
export bootloader_dir=openlib/$BUILD_PRO_NAME/bin
export project_dir=gccout/$OUTPUT_NAME/ap/$PROJECT_NAME
export CUSTOM_DIR=${BUILD_PRO_TARGET}_download

echo BUILD_PRO_NAME=$BUILD_PRO_NAME
echo BUILD_PRO_TARGET=$BUILD_PRO_TARGET
echo PROJECT_NAME=$PROJECT_NAME
echo project_dir=$project_dir
echo bootloader_dir=$bootloader_dir

echo ---------- Copy custom dowmload ------------
if [ -e $CUSTOM_DIR ]; then
    rm -rf $CUSTOM_DIR
fi
mkdir $CUSTOM_DIR

cp  $bootloader_dir/ap_bootloader.bin                    $CUSTOM_DIR/
cp  $bootloader_dir/ap_bootloader.elf                    $CUSTOM_DIR/
cp  $bootloader_dir/ap_bootloader.map                    $CUSTOM_DIR/

cp  $project_dir/comdb.txt                               $CUSTOM_DIR/
cp  $project_dir/ap_$PROJECT_NAME.bin                    $CUSTOM_DIR/
cp  $project_dir/ap_$PROJECT_NAME.elf                    $CUSTOM_DIR/
cp  $project_dir/ap_$PROJECT_NAME.map                    $CUSTOM_DIR/
cp  $project_dir/$PROJECT_NAME.binpkg                    $CUSTOM_DIR/$BUILD_PRO_NAME.binpkg

cp  ./mbtk/releasepack/$BUILD_PRO_NAME/config_*.ini      $CUSTOM_DIR/
cp  ./mbtk/releasepack/$BUILD_PRO_NAME/format*           $CUSTOM_DIR/
cp  ./mbtk/releasepack/$BUILD_PRO_NAME/*.bin             $CUSTOM_DIR/

echo ---------- Package end ------------
