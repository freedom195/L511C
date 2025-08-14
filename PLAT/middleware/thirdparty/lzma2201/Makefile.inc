
LZMA_DIR      := $(TOP)/PLAT/middleware/thirdparty/lzma2201/C
CFLAGS_INC    += -I $(TOP)/PLAT/os/freertos/inc               \
                  -I $(TOP)/PLAT/os/freertos/CMSIS/inc        \
                  -I $(TOP)/PLAT/middleware/thirdparty/cjson  \
                  -I $(LZMA_DIR) \
				  -I $(LZMA_DIR)/wrapper
              

LZMA_SRC_DIRS += $(LZMA_DIR) \
                 $(LZMA_DIR)/wrapper


LZMA_EXCLUDE_FILES :=

CFLAGS += -DFEATURE_LZMA_ENABLE

ifeq ($(TOOLCHAIN),GCC)

CFLAGS_INC += -I $(TOP)/PLAT/os/freertos/portable/gcc

$(BUILDDIR)/lib/liblzma.a: $(LZMA_COBJS)
	@mkdir -p $(dir $@)
	$(ECHO) AR $@
	$(Q)$(AR) -cr $@ $^

endif

ifeq ($(TOOLCHAIN),ARMCC)

CFLAGS_INC += -I $(TOP)/PLAT/os/freertos/portable/keil

lib-y += liblzma.lib

$(BUILDDIR)/lib/liblzma.lib: $(LZMA_COBJS)
	@mkdir -p $(dir $@)
	$(ECHO) AR $@
	$(Q)$(AR) $(ARFLAGS) $@ $^

endif
