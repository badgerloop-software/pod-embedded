CC            = g++
CFLAGS        = -Wall -Wextra
LIBS          = -lpthread -lrt

all: test

PracticalSocket.o: ../src/PracticalSocket.cpp
				$(CC) $(CFLAGS) -c $(INCLUDE) $<

TelemetrySender.o: ../src/TelemetrySender.cpp
				$(CC) $(CFLAGS) -c $(INCLUDE) $<
				
test.o: test.cpp
				$(CC) $(CFLAGS) -c $(INCLUDE) $<
				
i2c.o: ../../../embedded/xsens_imu/src/i2c.c ../../../embedded/xsens_imu/src/i2c-dev.h
				$(CC) $(CFLAGS) -c $(INCLUDE) $<

imu.o: ../../../embedded/xsens_imu/src/imu.c
				$(CC) $(CFLAGS) -c $(INCLUDE) $<
				
test: TelemetrySender.o PracticalSocket.o test.o imu.o i2c.o
				$(CC) $^ $(LIBS) -o test
								
clean:
				rm -rf *.o test
