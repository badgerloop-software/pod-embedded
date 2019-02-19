EXE = badgerloop
DRIVERS = drivers
PERIPHERALS = peripherals
EMBD_EXAMPLES = tests

DRIVER_SRC_DIR = embedded/drivers/src
PERIPHERAL_SRC_DIR = embedded/peripherals/src
EMBD_EXAMPLES_SRC_DIR = embedded/examples
MIDDLEWARE_SRC_DIR = middleware/src
MIDDLEWARE_EX_DIR = middleware/examples
APP_SRC_DIR = embedded/app/src

OBJ_DIR = $(OUTPUT_DIR)/obj
OBJ_DIR_DRIVER = $(OUTPUT_DIR)/obj
OBJ_DIR_PERIPHERAL = $(OUTPUT_DIR)/obj
OBJ_DIR_EXAMPLE = $(OUTPUT_DIR)/obj/tests
OBJ_DIR_MDL = $(OUTPUT_DIR)/obj
OBJ_DIR_MDL_EXAMPLE = $(OUTPUT_DIR)/obj/tests
OBJ_DIR_APP = $(OUTPUT_DIR)/obj

OUTPUT_DIR = bin

DRIVER_SRC = $(wildcard $(DRIVER_SRC_DIR)/*.c)
PERIPHERAL_SRC = $(wildcard $(PERIPHERAL_SRC_DIR)/*.c)
EMBD_EXAMPLES_SRC = $(wildcard $(EMBD_EXAMPLES_SRC_DIR)/*.c)
MDL_SRC = $(wildcard $(MIDDLEWARE_SRC_DIR)/*.cpp)
MDL_EXAMPLES_SRC = $(wildcard $(MIDDLEWARE_EX_DIR)/*.cpp)
APP_SRC = $(wildcard $(APP_SRC_DIR)/*.c)

DRIVER_OBJ := $(DRIVER_SRC:$(DRIVER_SRC_DIR)/%.c=$(OBJ_DIR_DRIVER)/%.o)
PERIPHERAL_OBJ := $(PERIPHERAL_SRC:$(PERIPHERAL_SRC_DIR)/%.c=$(OBJ_DIR_PERIPHERAL)/%.o)
EMBD_EXAMPLES_OBJ := $(EMBD_EXAMPLES_SRC:$(EMBD_EXAMPLES_SRC_DIR)/%.c=$(OBJ_DIR_EXAMPLE)/%.o)
MDL_OBJ := $(MDL_SRC:$(MIDDLEWARE_SRC_DIR)/%.cpp=$(OBJ_DIR_MDL)/%.o)
MDL_EXAMPLES_OBJ := $(MDL_EXAMPLES_SRC:$(MIDDLEWARE_EX_DIR)/%.cpp=$(OBJ_DIR_MDL_EXAMPLE)/%.o)
APP_OBJ := $(APP_SRC:$(APP_SRC_DIR)/%.c=$(OBJ_DIR_APP)/%.o)

EX_OUT = bin
EMBD_EX_OBJ_D = $(wildcard $(OBJ_DIR_EXAMPLE)/*.o)
EMBD_EXAMPLES_MAKE := $(EMBD_EX_OBJ_D:$(OBJ_DIR_EXAMPLE)/%.o=$(EX_OUT)/%)

GCC = gcc
GPP = g++
CPPFLAGS += -Iembedded/drivers/include -Iembedded/peripherals/include -Imiddleware/include -Imiddleware/include/jsonlib -Iembedded/data -Iembedded/app/include
CFLAGS += -Wall
CPFLAGS += -Wall -Wextra -std=c++11
LDFLAGS += -Llib
LDLIBS += -lm -lpthread

.PHONY: all clean tests

all: directories $(EXE)

examples: examples_make
	make examples_make

examples_make: example_directories $(EMBD_EXAMPLES) $(EMBD_EXAMPLES_MAKE)

directories: ${OBJ_DIR}

example_directories: ${OBJ_DIR_EXAMPLE}

${OBJ_DIR}:
	mkdir -p ${OBJ_DIR}
	
${OBJ_DIR_EXAMPLE}:
	mkdir -p ${OBJ_DIR_EXAMPLE}

$(EXE): $(DRIVER_OBJ) $(PERIPHERAL_OBJ) $(MDL_OBJ) $(APP_OBJ)
	$(GPP) $(LDFLAGS) $^ $(LDLIBS) -o $(OUTPUT_DIR)/$@
	
$(EMBD_EXAMPLES): $(DRIVER_OBJ) $(PERIPHERAL_OBJ) $(MDL_OBJ) $(EMBD_EXAMPLES_OBJ) $(MDL_EXAMPLES_OBJ)
	
$(EX_OUT)/%: $(OBJ_DIR_EXAMPLE)/%.o $(DRIVER_OBJ) $(PERIPHERAL_OBJ) $(MDL_OBJ) 
	$(GPP) $(LDFLAGS) $^ $(LDLIBS) -o $@


$(OBJ_DIR_DRIVER)/%.o: $(DRIVER_SRC_DIR)/%.c
	$(GCC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
	
$(OBJ_DIR_PERIPHERAL)/%.o: $(PERIPHERAL_SRC_DIR)/%.c
	$(GCC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
		
$(OBJ_DIR_EXAMPLE)/%.o: $(EMBD_EXAMPLES_SRC_DIR)/%.c
	$(GCC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR_MDL)/%.o: $(MIDDLEWARE_SRC_DIR)/%.cpp
	$(GPP) $(CPPFLAGS) $(CPFLAGS) -c $< -o $@
	
$(OBJ_DIR_MDL_EXAMPLE)/%.o: $(MIDDLEWARE_EX_DIR)/%.cpp
	$(GPP) $(CPPFLAGS) $(CPFLAGS) -c $< -o $@
	
$(OBJ_DIR_APP)/%.o: $(APP_SRC_DIR)/%.c
	$(GCC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OUTPUT_DIR)