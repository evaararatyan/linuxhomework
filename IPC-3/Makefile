CC = g++
CFLAGS = -std=c++17 -Wall
TARGET1 = first
TARGET2 = second
OBJECTS = shared_array.h

all: first second

first: first.cpp $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET1) first.cpp

second: second.cpp $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET2) second.cpp

clean:
	rm -f $(TARGET1) $(TARGET2)
