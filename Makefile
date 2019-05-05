VPATH := $(shell find . -name "src") ./embedded/app/main/ ./embedded/examples/ ./middleware/examples/ 

# Code and Includes (I know, shell commands everywhere! Works though)
ALL_SRC	:= $(shell find . -name "src" -exec ls {} \;)
ALL_EX	:= $(shell find . -name "examples" -exec ls {} \; | grep ".c")

# Should find all our include directories
INCLUDE_DIRS := $(shell find . -name "include") ./middleware/include/jsonlib

# Compiler options
GCC	   	:= gcc
GPP	   	:= g++
IFLAGS 	:= $(addprefix -I,$(INCLUDE_DIRS))
WFLAGS	:= -Wall -Wno-deprecated -Wextra -fdiagnostics-color=always
CFLAGS 	:= -std=gnu11
CPFLAGS := -std=c++11
LDFLAGS := -Llib
LDLIBS 	:= -lm -lpthread

# Output Control
OUTPUT_DIR 	:= out
OBJ_DIR	   	:= $(OUTPUT_DIR)/obj
EX_OUT_DIR	:= $(OUTPUT_DIR)/tests

HV_MAIN		:= badgerloop_HV
LV_MAIN		:= badgerloop_LV

TARGETS		:= $(addprefix $(OUTPUT_DIR)/, $(HV_MAIN) $(LV_MAIN))
EXAMPLES	:= $(addprefix $(EX_OUT_DIR)/,$(basename $(ALL_EX)))

# Each Example obj should have a main(); so it has to be linked into its own executable
GEN_OBJ		:= $(addprefix $(OBJ_DIR)/,$(addsuffix .o,$(basename $(ALL_SRC))))
EX_OBJ		:= $(addprefix $(OBJ_DIR)/,$(addsuffix .o,$(basename $(ALL_EX))))

.PHONY: all examples clean
# .SEC keeps intermediates, so make doesnt automatically clean .o files
.SECONDARY:

all: $(TARGETS)

examples: $(EXAMPLES)

$(OUTPUT_DIR)/%: $(GEN_OBJ) $(OBJ_DIR)/%.o
	$(GPP) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(EX_OUT_DIR)/%: $(GEN_OBJ) $(OBJ_DIR)/%.o | $(EX_OUT_DIR)
	$(GPP) $(LDFLAGS) $^ $(LDLIBS) -o $@

out/obj/%.o: %.c | $(OBJ_DIR)
	$(GCC) -c $(CFLAGS) $(IFLAGS) $(WFLAGS) $< -o $@

out/obj/%.o: %.cpp | $(OBJ_DIR)
	$(GPP) -c $(CPFLAGS) $(IFLAGS) $(WFLAGS) $< -o $@

$(OBJ_DIR): | $(OUTPUT_DIR)
	mkdir $(OBJ_DIR)

$(EX_OUT_DIR): | $(OUTPUT_DIR)
	mkdir $(EX_OUT_DIR)

$(OUTPUT_DIR):
	mkdir $(OUTPUT_DIR)

clean:
	-rm -rf $(OUTPUT_DIR)
  
