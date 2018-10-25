CC            = gcc
CFLAGS        = -Wall -Wextra

all: test

core.o: lib/core.c
				$(CC) $(CFLAGS) -c $(INCLUDE) $<

i2c.o: lib/i2c.c lib/i2c-dev.h
				$(CC) $(CFLAGS) -c $(INCLUDE) $<

imu.o: imu.c
				$(CC) $(CFLAGS) -c $(INCLUDE) $<
				
test: core.o i2c.o imu.o
				$(CC) $^ $(LIBS) -o test

clean:
				rm -rf *.o test
