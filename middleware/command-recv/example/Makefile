CC            = g++
CFLAGS        = -Wall -Wextra -std=c++11
LIBS          = -lpthread -lrt

all: test

LVTCPSocket.o: ../src/Low_Voltage/LVTCPSocket.cpp
				$(CC) $(CFLAGS) -c $(INCLUDE) $<
				
test.o: test.cpp
				$(CC) $(CFLAGS) -c $(INCLUDE) $<
				
test: LVTCPSocket.o test.o 
				$(CC) $^ $(LIBS) -o test
								
clean:
				rm -rf *.o test
