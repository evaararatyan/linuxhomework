CXX = g++
CXXFLAGS = -Wall -std=c++11

SRCDIR = src
BINDIR = bin

SRC = $(SRCDIR)/read-file.cpp
TARGET = $(BINDIR)/read-file

all: $(TARGET)

$(TARGET): $(SRC)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -rf $(BINDIR)

.PHONY: all clean

