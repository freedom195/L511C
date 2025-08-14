
AVAILABLE_TARGETS = ec7xx_0h00
TOOLCHAIN         = GCC
BINNAME           = ap_$(PROJECT)
TOP               := ../../../../../../..


#platform configuration, customers should not modify this file
include $(TOP)/PLAT/mbtk/target/$(BUILD_PRO_TARGET)/Makefile.inc

#---------------------------------------------------------------------------------

#custom change to open TTS / ZBAR
MBTK_OPENCPU_TTS  = n
MBTK_OPENCPU_ZBAR = n

#referencing customer third-party libraries
PREBUILDLIBS += ../lib/libtest.a

#---------------------------------------------------------------------------------

CFLAGS_INC        += -I ../inc \
                     -I $(TOP)/FIRMWARE/SRC/CAT1/Common/Inc \
                     -I $(TOP)/PLAT/driver/chip/$(CHIP)/ap/src/usb

#app project base code, cannot be deleted
obj-y             += PLAT/project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/app.o \
                     PLAT/project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/bsp_custom.o

#various function demos, customers can choose to add
obj-y             += PLAT/project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/app_demo_menu.o \
                     PLAT/project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/os_demo.o \
                     PLAT/project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/fs_demo.o \
                     PLAT/project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/time_demo.o \
                     PLAT/project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/sleep_demo.o \
                     PLAT/project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/uart_demo.o \
                     PLAT/project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/gpio_demo.o \
                     PLAT/project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/adc_demo.o \
                     PLAT/project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/i2c_demo.o \
                     PLAT/project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/spi_demo.o \
                     PLAT/project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/pwm_demo.o \
                     PLAT/project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/network_demo.o \
                     PLAT/project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/socket_demo.o \
                     PLAT/project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/ping_demo.o \
                     PLAT/project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/lbs_demo.o \
                     PLAT/project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/ftp_demo.o \
                     PLAT/project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/http_demo.o \
                     PLAT/project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/fota_demo.o \
                     PLAT/project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/ex_flash_demo.o


ifeq ($(MBTK_OPENCPU_AT), y)
obj-y   += PLAT/project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/at_demo.o
endif

ifeq ($(MBTK_OPENCPU_SMS), y)
obj-y   += PLAT/project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/sms_demo.o
endif

ifeq ($(MBTK_OPENCPU_TCP), y)
obj-y   += PLAT/project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/tcp_demo.o
endif

ifeq ($(MBTK_OPENCPU_SSL), y)
obj-y   += PLAT/project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/ssl_demo.o
endif

ifeq ($(THIRDPARTY_MQTT_ENABLE), y)
obj-y   += PLAT/project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/mqtt_demo.o
endif

ifeq ($(THIRDPARTY_CTWING1_5_ENABLE), y)
obj-y   += PLAT/project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/lwm2m_demo.o
endif


#---------------------------------------------------------------------------------
include $(TOP)/PLAT/tools/scripts/Makefile.rules

#Make all warnings into errors
CFLAGS += -Werror

ifneq ($(OPENCPU_MODE_ENABLE),y)
ifneq ($(BUILD_AT),y)
$(error This example needs to modify "BUILD_AT" to "y" in device\target\board\$(TARGET)\$(CORE)\$(TARGET)_$(CORE).mk)
endif
endif
