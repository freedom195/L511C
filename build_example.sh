#!/bin/bash
# 参数说明：
# $1 - params path: echo_app_top
# $2 - user cmd: build / clean / ...

print_not_null()
{
    if [ x"$1" = x"" ]; then
        return 1
    fi

    echo "$1"
}

set -e
cd `dirname $0`

BUILD_PARAM_DIR=$1
BUILD_PARAM_FILE=$BUILD_PARAM_DIR/build_param.config
. $BUILD_PARAM_FILE

EXAMPLE_NAME=$CONFIG_PROJECT_NAME
EXAMPLE_VER=$CONFIG_PROJECT_VERSION
HEADER_DIR=$OPEN_HEADER_DIR
LIBS_DIR=$OPEN_LIBS_DIR
LIBS=$PLATFORM_NEED_LIBS
OUTPUT_DIR=$BIN_OUTPUT_DIR
USER_CMD=$2
BOARD_NAME=$PLATFORM_BOARD

APP_BIN_NAME=$EXAMPLE_NAME
APP_VERSION=$EXAMPLE_VER

if [ "$USER_CMD" = "build" ]; then
    USER_CMD=all
fi

echo "Start Compile"
set -e
cd PLAT/
if [ "$USER_CMD" = "clean" ];then
	./build.sh clean
	echo "*************************************************************************"
	echo "************************CLEAN SUCCESS************************************"
	echo "*************************************************************************"
	exit 0
fi

export TUYA_LIB_DIR=$LIBS_DIR
export TUYA_LIBS="$LIBS"
export TUYA_APP_KCONFIG_DIR=${BUILD_PARAM_DIR}/../include
./build.sh

BIN_DIR=L511C_Y6E_download
if [ ! -d $OUTPUT_DIR ]; then
    mkdir $OUTPUT_DIR
fi
rm -rf $OUTPUT_DIR/*
cp $BIN_DIR $OUTPUT_DIR/ -r
cd $OUTPUT_DIR/
rm $BIN_DIR/config_factory_uart.ini $BIN_DIR/config_linux_uart.ini
cp $BIN_DIR/L511C_Y6E.binpkg $BIN_DIR/Custom.binpkg 
zip  $OUTPUT_DIR/${APP_BIN_NAME}_${APP_VERSION}.zip $BIN_DIR/*
cp $BIN_DIR/Custom.binpkg ${APP_BIN_NAME}_QIO_${APP_VERSION}.bin
cp $BIN_DIR/Custom.binpkg ${APP_BIN_NAME}_QIO_${APP_VERSION}.binpkg
