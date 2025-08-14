
OPENLIB_DIR   ?= $(TOP)/PLAT/openlib

PREBUILDLIBS += $(OPENLIB_DIR)/$(MBTK_PRODUCT_NAME)/lib/libdriver.a \
                $(OPENLIB_DIR)/$(MBTK_PRODUCT_NAME)/lib/libfreertos.a \
                $(OPENLIB_DIR)/$(MBTK_PRODUCT_NAME)/lib/libmiddleware_ec.a \
                $(OPENLIB_DIR)/$(MBTK_PRODUCT_NAME)/lib/libpsnv.a \
                $(OPENLIB_DIR)/$(MBTK_PRODUCT_NAME)/lib/libstartup.a \
                $(OPENLIB_DIR)/$(MBTK_PRODUCT_NAME)/lib/libtcpipmgr.a \
                $(OPENLIB_DIR)/$(MBTK_PRODUCT_NAME)/lib/libopenapi.a \
                $(OPENLIB_DIR)/$(MBTK_PRODUCT_NAME)/lib/libccio.a


ifeq ($(THIRDPARTY_YRCOMPRESS_ENABLE),y)
PREBUILDLIBS += $(OPENLIB_DIR)/$(MBTK_PRODUCT_NAME)/lib/libyrcompress.a
endif

ifeq ($(THIRDPARTY_BZIP2_ENABLE),y)
PREBUILDLIBS += $(OPENLIB_DIR)/$(MBTK_PRODUCT_NAME)/lib/libbzip2.a
endif

ifeq ($(THIRDPARTY_LZMA_ENABLE),y)
PREBUILDLIBS += $(OPENLIB_DIR)/$(MBTK_PRODUCT_NAME)/lib/liblzma.a
endif

ifeq ($(THIRDPARTY_MQTT_ENABLE),y)
PREBUILDLIBS += $(OPENLIB_DIR)/$(MBTK_PRODUCT_NAME)/lib/libmqtt.a
endif

ifeq ($(THIRDPARTY_HTTPC_ENABLE),y)
PREBUILDLIBS += $(OPENLIB_DIR)/$(MBTK_PRODUCT_NAME)/lib/libhttpclient.a
endif

ifeq ($(THIRDPARTY_LWIP_ENABLE),y)
PREBUILDLIBS += $(OPENLIB_DIR)/$(MBTK_PRODUCT_NAME)/lib/liblwip.a
endif

ifeq ($(THIRDPARTY_MBEDTLS_ENABLE),y)
PREBUILDLIBS += $(OPENLIB_DIR)/$(MBTK_PRODUCT_NAME)/lib/libmbedtls.a
endif

ifeq ($(THIRDPARTY_IPERF_ENABLE),y)
PREBUILDLIBS += $(OPENLIB_DIR)/$(MBTK_PRODUCT_NAME)/lib/libiperf.a
endif

ifeq ($(THIRDPARTY_PING_ENABLE),y)
PREBUILDLIBS += $(OPENLIB_DIR)/$(MBTK_PRODUCT_NAME)/lib/libping.a
endif

ifeq ($(THIRDPARTY_LITTEFS_ENABLE),y)
PREBUILDLIBS += $(OPENLIB_DIR)/$(MBTK_PRODUCT_NAME)/lib/liblfs.a
endif

ifeq ($(THIRDPARTY_LIBSNTP_ENABLE),y)
PREBUILDLIBS += $(OPENLIB_DIR)/$(MBTK_PRODUCT_NAME)/lib/libsntp.a
endif

ifeq ($(THIRDPARTY_CJSON_ENABLE),y)
PREBUILDLIBS += $(OPENLIB_DIR)/$(MBTK_PRODUCT_NAME)/lib/libcjson.a
endif

ifeq ($(findstring y, $(THIRDPARTY_CMCC_DM_ENABLE)$(THIRDPARTY_CTCC_DM_ENABLE)$(THIRDPARTY_CUCC_DM_ENABLE)), y)
PREBUILDLIBS += $(OPENLIB_DIR)/$(MBTK_PRODUCT_NAME)/lib/libdm.a
endif

ifeq ($(THIRDPARTY_CTWING_CERTI_ENABLE),y)
PREBUILDLIBS += $(OPENLIB_DIR)/$(MBTK_PRODUCT_NAME)/lib/libctwing.a
endif

ifeq ($(findstring y, $(THIRDPARTY_WAKAAMA_ENABLE)$(THIRDPARTY_CTWING1_5_ENABLE)), y)
PREBUILDLIBS += $(OPENLIB_DIR)/$(MBTK_PRODUCT_NAME)/lib/libwakaamacore.a
endif

ifeq ($(THIRDPARTY_CTWING1_5_ENABLE),y)
PREBUILDLIBS += $(OPENLIB_DIR)/$(MBTK_PRODUCT_NAME)/lib/libctlwm2m.a
endif

ifeq ($(THIRDPARTY_ERCOAP_ENABLE),y)
PREBUILDLIBS += $(OPENLIB_DIR)/$(MBTK_PRODUCT_NAME)/lib/libercoap.a
endif

ifeq ($(THIRDPARTY_UECC_ENABLE),y)
PREBUILDLIBS += $(OPENLIB_DIR)/$(MBTK_PRODUCT_NAME)/lib/libuecc.a
endif

