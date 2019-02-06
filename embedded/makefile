# A super ham fisted start at an overall makefile. I swear it'll get better when
# I learn more! Mostly to take a stab at getting all of our includes to work
# and get something going. Im just sorry for the state its in now and for my
# variable naming. 	- Ezra Boley

CC			 := gcc
INC_DIRS     := drivers/include app/include peripherals/include examples/include
DBG_INC_DIR  := examples/include #Find a way to work this in instead of above
INC_OPTS	 := $(foreach d, $(INC_DIRS), -I$d)

OBJS		 := #Not sure yet. 
EXEC		 := badgerloop
CFLAGS 		 := $(INC_OPTS) -o $(EXEC)


all: 
	$(CC) $(ARGS) $(OBJS)

i2c.o:

can.o: can.c
