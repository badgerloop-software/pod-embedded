EXE = badgerloop
DRIVERS = drivers
PERIPHERALS = peripherals
EMBD_EXAMPLES = tests

DRIVER_SRC_DIR = embedded/drivers/src
PERIPHERAL_SRC_DIR = embedded/peripherals/src
EMBD_EXAMPLES_SRC_DIR = embedded/examples

OBJ_DIR = $(OUTPUT_DIR)/obj
OBJ_DIR_DRIVER = $(OUTPUT_DIR)/obj
OBJ_DIR_PERIPHERAL = $(OUTPUT_DIR)/obj
OBJ_DIR_EXAMPLE = $(OUTPUT_DIR)/obj/tests
OUTPUT_DIR = bin

DRIVER_SRC = $(wildcard $(DRIVER_SRC_DIR)/*.c)
PERIPHERAL_SRC = $(wildcard $(PERIPHERAL_SRC_DIR)/*.c)
EMBD_EXAMPLES_SRC = $(wildcard $(EMBD_EXAMPLES_SRC_DIR)/*.c)


DRIVER_OBJ := $(DRIVER_SRC:$(DRIVER_SRC_DIR)/%.c=$(OBJ_DIR_DRIVER)/%.o)
PERIPHERAL_OBJ := $(PERIPHERAL_SRC:$(PERIPHERAL_SRC_DIR)/%.c=$(OBJ_DIR_PERIPHERAL)/%.o)
EMBD_EXAMPLES_OBJ := $(EMBD_EXAMPLES_SRC:$(EMBD_EXAMPLES_SRC_DIR)/%.c=$(OBJ_DIR_EXAMPLE)/%.o)

EX_OUT = bin
EMBD_EX_OBJ_D = $(wildcard $(OBJ_DIR_EXAMPLE)/*.o)
EMBD_EXAMPLES_MAKE := $(EMBD_EX_OBJ_D:$(OBJ_DIR_EXAMPLE)/%.o=$(EX_OUT)/%)

CC = gcc
CPPFLAGS += -Iembedded/drivers/include -Iembedded/peripherals/include
CFLAGS += -Wall
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

$(EXE): $(DRIVER_OBJ) $(PERIPHERAL_OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $(OUTPUT_DIR)/$@
	
$(EMBD_EXAMPLES): $(DRIVER_OBJ) $(PERIPHERAL_OBJ) $(EMBD_EXAMPLES_OBJ)
	
$(EX_OUT)/%: $(OBJ_DIR_EXAMPLE)/%.o $(DRIVER_OBJ) $(PERIPHERAL_OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@


$(OBJ_DIR_DRIVER)/%.o: $(DRIVER_SRC_DIR)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
	
$(OBJ_DIR_PERIPHERAL)/%.o: $(PERIPHERAL_SRC_DIR)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
		
$(OBJ_DIR_EXAMPLE)/%.o: $(EMBD_EXAMPLES_SRC_DIR)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OUTPUT_DIR)