
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
	$(wildcard ./gfx/src/*.cpp) \
	$(wildcard ./gfx/fontPainter/fonts/*.c) \
	./gfx/fontPainter/knownFonts.c \
	

