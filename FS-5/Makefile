CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

TARGET = manual-append
OBJECTS = manual-append.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

manual-append.o: manual-append.cpp
	$(CXX) $(CXXFLAGS) -c manual-append.cpp -o manual-append.o

clean:
	rm -f $(OBJECTS) $(TARGET)
