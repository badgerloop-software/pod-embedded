CC            = gcc
CFLAGS        = -Wall -Wextra
LIBS          = -lpthread -lrt

all: test

i2c.o: ../src/i2c.c ../src/i2c-dev.h
				$(CC) $(CFLAGS) -c $(INCLUDE) $<

imu.o: ../src/imu.c
				$(CC) $(CFLAGS) -c $(INCLUDE) $<
				
test.o: test.c
				$(CC) $(CFLAGS) -c $(INCLUDE) $<
				
test: i2c.o imu.o test.o
				$(CC) $^ $(LIBS) -o test

clean:
				rm -rf *.o test
