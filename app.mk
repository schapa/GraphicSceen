
CFLAGS += \
	-I./dbg \
	-I./gfx \
	-I./gfx/sprites \
	-I./gfx/fontPainter \
	-I./gfx/fontPainter/fonts \
	-I./inc \
	-I./Queue \
	-I./menu \
	
CFLAGS += \
	-DTRACE \
	-DSTM32F429xx \
	-DHSE_VALUE=8000000 \
	-I./sdk/include/ \
	-I./sdk/include/arm \
	-I./sdk/include/cmsis/ \
	-I./sdk/include/cortexm/ \
	-I./sdk/include/diag \
	-I./sdk/include/stm32f4-hal \
	
export SRC := \
	./dbg/dbg_trace.c \
	./src/main.cpp \
	./src/memman.c \
	./src/Queue.c \
	./src/ssd1322.c \
	./src/system.c \
	./src/_initialize_hardware.c \
	./src/_write.c \
	./src/stm32f4xx_hal_msp.c \
	\
	./sdk/src/cortexm/exception_handlers.c \
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
	./sdk/src/stm32f4-hal/stm32f4xx_hal_can.c \
	./sdk/src/stm32f4-hal/stm32f4xx_hal_tim.c \
	./sdk/src/stm32f4-hal/stm32f4xx_hal_tim_ex.c \
	./sdk/src/stm32f4-hal/stm32f4xx_hal_ltdc.c \
	\
	$(wildcard ./gfx/src/*.cpp) \
	$(wildcard ./gfx/sprites/*.cpp) \
	$(wildcard ./gfx/fontPainter/fonts/*.c) \
	./gfx/fontPainter/knownFonts.c \
	

