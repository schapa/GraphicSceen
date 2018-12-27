# toolchain defs
include $(ROOT_DIR)/toolchain.mk

OUT_SFX := app
BUILD_DIR :=$(BUILD_DIR)/$(OUT_SFX)

PROGNAME := scr-$(OUT_SFX)
FIRMWARE := scr-$(OUT_SFX).hex
MAP_FILE := $(BUILD_ROOT)/$(PROGNAME).map

# firmware specific sources
export ASM_SRC := 

SRC += \
	$(wildcard ./firmware/*.c) \
	
SRC += \
	./sdk/src/cortexm/exception_handlers.c \
	\
	./sdk/src/cmsis/system_stm32f4xx.c \
	./sdk/src/cmsis/vectors_stm32f429xx.c \
	./sdk/src/cortexm/_initialize_hardware.c \
	./sdk/src/cortexm/_reset_hardware.c \
	./sdk/src/cortexm/exception_handlers.c \
	\
	./sdk/src/diag/Trace.c \
	./sdk/src/diag/trace_impl.c \
	\
	./sdk/src/newlib/_cxx.cpp \
	./sdk/src/newlib/_exit.c \
	./sdk/src/newlib/_sbrk.c \
	./sdk/src/newlib/_startup.c \
	./sdk/src/newlib/_syscalls.c \
	./sdk/src/newlib/assert.c \
	\

CFLAGS += \
	-I./firmware/ \

CFLAGS += \
	-Og \
	-ggdb \
	-mcpu=cortex-m4 \
	-mthumb -mabi=aapcs \
	-mfloat-abi=soft \
	-ffunction-sections \
	-fdata-sections \

LDFLAGS += \
	-mcpu=cortex-m4 \
	-mthumb \
	-fmessage-length=0 \
	-T mem.ld -T libs.ld -T sections.ld \
	-nostartfiles \
	-Xlinker --gc-sections \
	-L"./ldscripts" \
	-Wl,-Map,"$(MAP_FILE)" \
	--specs=nano.specs \

.PHONY: all clean info

all: info $(FIRMWARE)

info:
	@echo "[BUILD] firmware"

$(FIRMWARE): $(PROGNAME)
	@echo " [FW] $< -> $@"
	$(OBJCOPY) -O ihex "$(BUILD_ROOT)/$(PROGNAME)" "$(BUILD_ROOT)/$(PROGNAME).hex"
	@echo " [Size]"
	$(SIZE) --format=berkeley "$(BUILD_ROOT)/$(PROGNAME)"

clean::
	@echo "[CLEAN] $(PROGNAME)"
	rm -f $(BUILD_ROOT)/$(PROGNAME) $(BUILD_ROOT)/$(FIRMWARE) $(BUILD_ROOT)/$(MAP_FILE)

include $(ROOT_DIR)/build.mk
