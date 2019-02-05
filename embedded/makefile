# A super ham fisted start at an overall makefile. I swear it'll get better when
# I learn more! Mostly to take a stab at getting all of our includes to work
# and get something going. Im just sorry for the state its in now and for my
# variable naming. 	- Ezra Boley

CC			 = gcc
INC_DIRS     = drivers/include app/include peripherals/include
DBG_INC_DIR  = examples/include
INC_OPTS	 = $(foreach d, $(INC_DIRS), -I$d)
OBJS		 = #Not sure yet. 
OUT			 = -o badgerloop
ARGS 		 = $(INC_OPTS) $(OUT)


all: 
	$(CC) $(ARGS) $(OBJS)
