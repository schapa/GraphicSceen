
export CPP := g++
export LD := g++

OUT_SFX := ui
BUILD_DIR :=$(BUILD_DIR)/$(OUT_SFX)

PROGNAME := gui

CFLAGS += \
	-I./emulator/ \
	
SRC := \
	./emulator/ui.c

# emulator specific CFLAGS
CFLAGS += \
	-ggdb \
	-O0 \
	-DEMULATOR \
	-Wno-deprecated-declarations \
	-Wall -Wformat=0 \
	$(shell pkg-config --cflags gtk+-3.0) \

# emulator specific LIBS
LDLIBS += \
	-lc \
	-lpthread \
	$(shell pkg-config --libs gtk+-3.0) \

.PHONY: all clean info

all: info $(PROGNAME)

info:
	@echo "[BUILD] $(PROGNAME)"

clean::
	@echo "[CLEAN] $(PROGNAME)"
	rm -rf $(BUILD_ROOT)/$(PROGNAME)

include $(ROOT_DIR)/build.mk
