
CFLAGS += \
	-I./dbg \
	-I./gfx \
	-I./gfx/fontPainter \
	-I./gfx/fontPainter/fonts \
	-I./inc \
	-I./Queue \
	-I./menu \
	
CFLAGS += \
	-DTRACE \
	-DSTM32F429xx \
	-DHSE_VALUE=8000000 \
	-I./system/include/ \
	-I./system/include/arm \
	-I./system/include/cmsis/ \
	-I./system/include/cortexm/ \
	-I./system/include/diag \
	-I./system/include/stm32f4-hal \
	
export SRC := \
	./dbg/dbg_trace.c \
	./src/main.cpp \
	./src/memman.c \
	./src/Queue.c \
	./src/ssd1322.c \
	./src/system.c \
	\
	./sys/_initialize_hardware.c \
	./sys/_write.c \
	./sys/stm32f4xx_hal_msp.c \
	\
	./system/src/stm32f4-hal/stm32f4xx_hal.c \
	./system/src/stm32f4-hal/stm32f4xx_hal_cortex.c \
	./system/src/stm32f4-hal/stm32f4xx_hal_rcc.c \
	./system/src/stm32f4-hal/stm32f4xx_hal_rcc_ex.c \
	./system/src/stm32f4-hal/stm32f4xx_hal_gpio.c \
	./system/src/stm32f4-hal/stm32f4xx_hal_dma.c \
	./system/src/stm32f4-hal/stm32f4xx_hal_usart.c \
	./system/src/stm32f4-hal/stm32f4xx_hal_uart.c \
	./system/src/stm32f4-hal/stm32f4xx_hal_spi.c \
	./system/src/stm32f4-hal/stm32f4xx_hal_can.c \
	./system/src/stm32f4-hal/stm32f4xx_hal_tim.c \
	./system/src/stm32f4-hal/stm32f4xx_hal_tim_ex.c \
	./system/src/stm32f4-hal/stm32f4xx_hal_ltdc.c \
	\
	$(wildcard ./gfx/src/*.cpp) \
	$(wildcard ./gfx/fontPainter/fonts/*.c) \
	./gfx/fontPainter/knownFonts.c \
	

