
export CPP := g++
export LD := g++

OUT_SFX := emulator
BUILD_DIR :=$(BUILD_DIR)/$(OUT_SFX)

PROGNAME := scr-$(OUT_SFX)

CFLAGS += \
	-I./emulator/ \
	
SRC += \
	./emulator/bsp.c \
	./emulator/system.c \
	./emulator/emulator.c \
	./emulator/cortex.c \

# emulator specific CFLAGS
CFLAGS += \
	-g \
	-O0 \
	-m32 \
	-DEMULATOR \
	-I./emulator/ \
	-Wno-deprecated-declarations \
	-Wall -Wformat=0 \

# emulator specific LIBS
LDLIBS += \
	-m32 \
	-lc \
	-lpthread \
	-lrt

.PHONY: all clean info

all: info $(PROGNAME)

info:
	@echo "[BUILD] $(PROGNAME)"

clean::
	@echo "[CLEAN] $(PROGNAME)"
	rm -rf $(BUILD_ROOT)/$(PROGNAME)

include $(ROOT_DIR)/build.mk
