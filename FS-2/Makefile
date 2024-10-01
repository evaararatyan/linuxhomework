CXX = g++
CXXFLAGS = -Wall -std=c++11

SRCDIR = src
BINDIR = bin

SRC = $(SRCDIR)/simple-copy.cpp
TARGET = $(BINDIR)/simple-copy

all: $(TARGET)

$(TARGET): $(SRC)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -rf $(BINDIR)

.PHONY: all clean
