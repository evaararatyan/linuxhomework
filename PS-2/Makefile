CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++11

TARGET := interactive-shell

SRC := interactive-shell.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean
