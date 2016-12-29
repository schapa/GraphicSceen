export CROSS_COMPILE := arm-none-eabi-

export AS      := $(CROSS_COMPILE)as
export CC      := $(CROSS_COMPILE)gcc
export CPP     := $(CROSS_COMPILE)g++
export LD      := $(CROSS_COMPILE)g++
export OBJCOPY := $(CROSS_COMPILE)objcopy
export SIZE    := $(CROSS_COMPILE)size
