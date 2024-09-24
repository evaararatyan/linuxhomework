CXX = g++
CXXFLAGS = -Iinclude -c

SRC = src/main.cpp src/complex.cpp src/sort.cpp
OBJ = $(SRC:.cpp=.o)

TARGET = program

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

src/main.o: src/main.cpp include/complex.h include/sort.h
	$(CXX) $(CXXFLAGS) src/main.cpp -o src/main.o

src/complex.o: src/complex.cpp include/complex.h
	$(CXX) $(CXXFLAGS) src/complex.cpp -o src/complex.o

src/sort.o: src/sort.cpp include/sort.h
	$(CXX) $(CXXFLAGS) src/sort.cpp -o src/sort.o

clean:
	rm -f src/*.o $(TARGET)

