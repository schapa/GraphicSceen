# requires
# SRC      - C sources list
# ASM_SRC  - C sources list
# PROGNAME - elf output name
#
# optional
# BUILD_DIR - output files directory
#
# provides targets:
#   clean
C_SRC := $(filter-out %.c, $(SRC))
CPP_SRC := $(filter-out %.cpp, $(SRC))

OBJ := $(CPP_SRC:.c=.o)
OBJ += $(C_SRC:.cpp=.o)
OBJ += $(ASM_SRC:.S=.o)

DEP := $(OBJ:.o=.d)
DEP := $(addprefix $(BUILD_DIR)/, $(DEP))

VPATH = $(BUILD_DIR) $(BUILD_DIR)/..

$(PROGNAME): $(addprefix $(BUILD_DIR)/, $(OBJ))
	@echo " [LD]" $(PROGNAME)
	$(LD) $(LDFLAGS) $^ $(LDLIBS) -o $(BUILD_ROOT)/$(PROGNAME)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(@D)
	@echo " [CC]" $<
	$(CC) $(CFLAGS) $(CCFLAGS) -c -MD -MP $< -o $@
	
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	@echo " [CPP]" $<
	$(CPP) $(CFLAGS) $(CXXFLAGS) -c -MD -MP $< -o $@

$(BUILD_DIR)/%.o: %.S
	@mkdir -p $(@D)
	@echo " [AS]" $<
	$(CC) $(CFLAGS) -c -MD -MP $< -o $@

# double colon is required here in order to be able
# to extend 'clean' target  in including file
clean::
	rm -rf $(BUILD_DIR) $(PROGNAME)

-include $(DEP)
