CC            = gcc
CFLAGS        = -Wall -Wextra
LIBS          = -lpthread -lrt

all: test

i2c.o: ../lib/i2c.c ../lib/i2c-dev.h
				$(CC) $(CFLAGS) -c $(INCLUDE) $<

imu.o: ../lib/imu.c
				$(CC) $(CFLAGS) -c $(INCLUDE) $<
				
test.o: test.c
				$(CC) $(CFLAGS) -c $(INCLUDE) $<
				
test: i2c.o imu.o test.o
				$(CC) $^ $(LIBS) -o test

clean:
				rm -rf *.o test
