EXE = badgerloop
DRIVERS = drivers
PERIPHERALS = peripherals
EXAMPLES = examples

DRIVER_SRC_DIR = drivers/src
PERIPHERAL_SRC_DIR = peripherals/src
EXAMPLES_SRC_DIR = examples

OBJ_DIR = $(OUTPUT_DIR)obj
OBJ_DIR_DRIVER = $(OUTPUT_DIR)obj
OBJ_DIR_PERIPHERAL = $(OUTPUT_DIR)obj
OBJ_DIR_EXAMPLE = $(OUTPUT_DIR)obj/tests
OUTPUT_DIR = bin/

DRIVER_SRC = $(wildcard $(DRIVER_SRC_DIR)/*.c)
PERIPHERAL_SRC = $(wildcard $(PERIPHERAL_SRC_DIR)/*.c)
EXAMPLES_SRC = $(wildcard $(EXAMPLES_SRC_DIR)/*.c)


DRIVER_OBJ := $(DRIVER_SRC:$(DRIVER_SRC_DIR)/%.c=$(OBJ_DIR_DRIVER)/%.o)
PERIPHERAL_OBJ := $(PERIPHERAL_SRC:$(PERIPHERAL_SRC_DIR)/%.c=$(OBJ_DIR_PERIPHERAL)/%.o)
EXAMPLES_OBJ := $(EXAMPLES_SRC:$(EXAMPLES_SRC_DIR)/%.c=$(OBJ_DIR_EXAMPLE)/%.o)

CC = gcc
CPPFLAGS += -Idrivers/include -Iperipherals/include
CFLAGS += -Wall
LDFLAGS += -Llib
LDLIBS += -lm -lpthread

.PHONY: all clean

all: directories $(EXE)

tests: example_directories $(EXAMPLES) $(EXAMPLES_MAKE)

directories: ${OBJ_DIR}

example_directories: ${OBJ_DIR_EXAMPLE}

${OBJ_DIR}:
	mkdir -p ${OBJ_DIR}
	
${OBJ_DIR_EXAMPLE}:
	mkdir -p ${OBJ_DIR_EXAMPLE}

$(EXE): $(DRIVER_OBJ) $(PERIPHERAL_OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $(OUTPUT_DIR)$@
	
$(EXAMPLES): $(DRIVER_OBJ) $(PERIPHERAL_OBJ) $(EXAMPLES_OBJ)
#	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $(OUTPUT_DIR)$@


$(OBJ_DIR_DRIVER)/%.o: $(DRIVER_SRC_DIR)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
	
$(OBJ_DIR_PERIPHERAL)/%.o: $(PERIPHERAL_SRC_DIR)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
		
$(OBJ_DIR_EXAMPLE)/%.o: $(EXAMPLES_SRC_DIR)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OUTPUT_DIR)