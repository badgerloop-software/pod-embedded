# Because I guess paths in Makefiles are black magic:
EZRA :=
WAS	 := $(EZRA) $(EZRA)
HERE := $(shell find -name "src")

VPATH := $(subst $(WAS),:,$(HERE))
VPATH += :./embedded/app/main/:./embedded/examples/:./middleware/examples/

# Code and Includes (I know, my grep game is weak)
ALL_C	:= $(shell find -name "*.c")
ALL_CPP := $(shell find -name "*.cpp")



ALL_SRC		:= $(shell find | grep "src" | grep "\.c")
ALL_C_SRC	:= $(shell find | grep "src" | grep "\.c" | grep -v "\.cpp")
ALL_CPP_SRC := $(shell find | grep "src" | grep "\.c" | grep "\.cpp")

ALL_C_EX	:= $(shell find | grep "examples" | grep "\.c" | grep -v "\.cpp")
ALL_CPP_EX  := $(shell find | grep "examples" | grep "\.c" | grep "\.cpp")

INCLUDE_DIRS := $(shell find -name "include")

# Compiler options
GCC	   	:= gcc
GPP	   	:= g++
IFLAGS 	:= $(addprefix -I,$(INCLUDE_DIRS))
WFLAGSS	:= -Wall -Wno-deprecated -Wextra 
CFLAGS 	:= -std=gnu11
CPFLAGS := -std=c++11
LDFLAGS := -Llib
LDLIBS 	:= -lm -lpthread

# Output Control
OUTPUT_DIR 	:= out
HV_MAIN		:= badgerloop_hv
LV_MAIN		:= badgerloop_lv
OBJ_DIR	   	:= $(OUTPUT_DIR)/obj
ALL_OBJ	   	:= $(shell find -name "*.c*" -type f -exec basename {} \;)
ALL_OBJ	   	:= $(ALL_OBJ:%.c=%.o)
ALL_OBJ		:= $(addprefix $(OBJ_DIR)/, $(ALL_OBJ:%.cpp=%.o))


# Examples
.PHONY: all scan clean $(HV_MAIN) $(LV_MAIN)

test: 
	echo $(VPATH)

all: $(HV_MAIN) $(LV_MAIN)

$(HV_MAIN): $(ALL_OBJ)
	$(GPP) -o $@ $(ALL_OBJ)

$(LV_MAIN): $(ALL_OBJ)
	$(GPP) -o $@ $(ALL_OBJ)

out/obj/%.o: %.c | $(OBJ_DIR)
	$(GCC) -c $(CFLAGS) $(IFLAGS) $(WFLAGS) $(LDFLAGS $(LDLIBS) $< -o $@

out/obj/%.o: %.cpp | $(OBJ_DIR)
	$(GPP) -c $(CPFLAGS) $(IFLAGS) $(WFLAGS) $(LDFLAGS) $(LDLIBS) $< -o $@

$(OBJ_DIR): $(OUTPUT_DIR)
	mkdir $(OBJ_DIR)

$(OUTPUT_DIR):
	mkdir $(OUTPUT_DIR)

scan: 
	scan-build make > scan_build_out.log
	
clean:
	rm -rf $(OUTPUT_DIR)
