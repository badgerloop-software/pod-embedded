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
				
test: TelemetrySender.o PracticalSocket.o test.o
				$(CC) $^ $(LIBS) -o test

clean:
				rm -rf *.o test
