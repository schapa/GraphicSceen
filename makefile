export ROOT_DIR := .
export BUILD_DIR := ./build
export BUILD_ROOT :=$(BUILD_DIR)


# common CFLAGS and LDFLAGS, BUILD_TYPE (debug, release)
include common-defs.mk

include app.mk

.PHONY: all firmware emulator clean

all: config firmware emulator

config:
	@echo "=== Build config =================="
	@echo " BUILD_TYPE = $(BUILD_TYPE)"
	@echo "=================================="

firmware emulator:
	$(MAKE) -f $@.mk

clean::
	$(MAKE) -f firmware.mk $@
	$(MAKE) -f emulator.mk $@
	
include build.mk
