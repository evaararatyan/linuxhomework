CXX = g++
CXXFLAGS = -Wall -std=c++11

SRCDIR = src
BINDIR = bin

SRC = $(SRCDIR)/hacker-rm.cpp
TARGET = $(BINDIR)/hacker-rm

all: $(TARGET)

$(TARGET): $(SRC)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -rf $(BINDIR)

.PHONY: all clean
