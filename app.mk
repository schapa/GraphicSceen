
LVGL_DIR = lvgl


CFLAGS += \
	-I./dbg \
	-I./gfx \
	-I./gfx/sprites \
	-I./gfx/fontPainter \
	-I./gfx/fontPainter/fonts \
	-I./inc \
	-I./lib \
	-I./Queue \
	-I./menu \
	\
	-I$(LVGL_DIR)/ \
	-I$(LVGL_DIR)/src/lv_core \
	-I$(LVGL_DIR)/src/lv_draw \
	-I$(LVGL_DIR)/src/lv_font \
	-I$(LVGL_DIR)/src/lv_hal \
	-I$(LVGL_DIR)/src/lv_misc \
	-I$(LVGL_DIR)/src/lv_objx \
	-I$(LVGL_DIR)/src/lv_porting \
	-I$(LVGL_DIR)/src/lv_themes \
	
CFLAGS += \
	-DTRACE \
	-DSTM32F429xx \
	-DUSE_HAL_DRIVER \
	-DHSE_VALUE=8000000 \
	-I./sdk/include/ \
	-I./sdk/include/arm \
	-I./sdk/include/cmsis/ \
	-I./sdk/include/cortexm/ \
	-I./sdk/include/diag \
	-I./sdk/include/stm32f4-hal \
	-ffunction-sections \
	-fdata-sections \
	\
	-Wno-narrowing \
	-Wno-sign-compare \

CCFLAGS += \
	-Wno-pointer-to-int-cast \

export SRC := \
	./dbg/dbg_trace.c \
	$(wildcard ./src/*.c) \
	$(wildcard ./src/*.cpp) \
	$(wildcard ./lib/src/*.c) \
	\
	\
	./sdk/src/stm32f4-hal/stm32f4xx_hal.c \
	./sdk/src/stm32f4-hal/stm32f4xx_hal_cortex.c \
	./sdk/src/stm32f4-hal/stm32f4xx_hal_rcc.c \
	./sdk/src/stm32f4-hal/stm32f4xx_hal_rcc_ex.c \
	./sdk/src/stm32f4-hal/stm32f4xx_hal_gpio.c \
	./sdk/src/stm32f4-hal/stm32f4xx_hal_dma.c \
	./sdk/src/stm32f4-hal/stm32f4xx_hal_usart.c \
	./sdk/src/stm32f4-hal/stm32f4xx_hal_uart.c \
	./sdk/src/stm32f4-hal/stm32f4xx_hal_spi.c \
	./sdk/src/stm32f4-hal/stm32f4xx_hal_i2c.c \
	./sdk/src/stm32f4-hal/stm32f4xx_hal_can.c \
	./sdk/src/stm32f4-hal/stm32f4xx_hal_tim.c \
	./sdk/src/stm32f4-hal/stm32f4xx_hal_tim_ex.c \
	./sdk/src/stm32f4-hal/stm32f4xx_hal_ltdc.c \
	./sdk/src/stm32f4-hal/stm32f4xx_ll_fmc.c \
	./sdk/src/stm32f4-hal/stm32f4xx_hal_sdram.c \
	\
	$(wildcard ./gfx/src/*.cpp) \
	$(wildcard ./gfx/sprites/*.cpp) \
	$(wildcard ./gfx/fontPainter/fonts/*.c) \
	./gfx/fontPainter/knownFonts.c \
	\
	$(wildcard $(LVGL_DIR)/src/lv_core/*.c*) \
	$(wildcard $(LVGL_DIR)/src/lv_draw/*.c*) \
	$(wildcard $(LVGL_DIR)/src/lv_font/*.c*) \
	$(wildcard $(LVGL_DIR)/src/lv_hal/*.c*) \
	$(wildcard $(LVGL_DIR)/src/lv_misc/*.c*) \
	$(wildcard $(LVGL_DIR)/src/lv_objx/*.c*) \
	$(wildcard $(LVGL_DIR)/src/lv_porting/*.c*) \
	$(wildcard $(LVGL_DIR)/src/lv_themes/*.c*) \

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	LDFLAGS += \
		-Wl,--wrap=malloc \
		-Wl,--wrap=free
endif

