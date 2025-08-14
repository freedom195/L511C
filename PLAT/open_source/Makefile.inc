
ifeq ($(MBTK_OPENCPU_TTS),y)
include $(TOP)/PLAT/open_source/tts/Makefile.inc
endif

ifeq ($(MBTK_OPENCPU_ZBAR),y)
include $(TOP)/PLAT/open_source/zbar/Makefile.inc
endif
