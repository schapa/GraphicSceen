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
	./firmware/bsp.c \
	./firmware/bspLcd.c \
	./firmware/canWrapper.c \
	./firmware/pwmWrapper.c \
	./firmware/system.c \
	./firmware/tracer.c \
	./firmware/DiscoLcd_ili9341.c \
	
SRC += \
	./sys/_initialize_hardware.c \
	./sys/_write.c \
	./sys/stm32f4xx_hal_msp.c \
	./system/src/cmsis/system_stm32f4xx.c \
	./system/src/cmsis/vectors_stm32f429xx.c \
	./system/src/cortexm/_initialize_hardware.c \
	./system/src/cortexm/_reset_hardware.c \
	./system/src/cortexm/exception_handlers.c \
	\
	./system/src/diag/Trace.c \
	./system/src/diag/trace_impl.c \
	\
	./system/src/newlib/_cxx.cpp \
	./system/src/newlib/_exit.c \
	./system/src/newlib/_sbrk.c \
	./system/src/newlib/_startup.c \
	./system/src/newlib/_syscalls.c \
	./system/src/newlib/assert.c \
	\

CFLAGS += \
	-I./firmware/ \

CFLAGS += -Og \
	-ggdb \
	-mcpu=cortex-m4 \
	-mthumb -mabi=aapcs \
	-mfloat-abi=soft \

LDFLAGS += \
	-mcpu=cortex-m4 \
	-mthumb \
	-fmessage-length=0 \
	-fsigned-char \
	-ffunction-sections \
	-fdata-sections \
	-ffreestanding \
	-fno-move-loop-invariants \
	-Wall \
	-Wextra \
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
