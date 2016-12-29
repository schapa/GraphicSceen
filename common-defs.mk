
export CFLAGS := \
	-DBUILD_VERSION=$(BUILD_VERSION) \
	-Wall -Werror \
	
export CCFLAGS := \
	-std=gnu99 \
	
export CXXFLAGS := \
	-std=c++11 \

export LDFLAGS :=

export LDLIBS :=

ifeq ($(BUILD_TYPE), debug)
CFLAGS += -DDEBUG
else
CFLAGS += -DRELEASE
endif # BUILD_TYPE

CFLAGS += -Wfatal-errors

ifndef VERBOSE
.SILENT:
endif
