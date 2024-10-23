CXX = g++
CXXFLAGS = -Wall -g
TARGET = do-command

all: $(TARGET)

$(TARGET): do_command.o
	$(CXX) $(CXXFLAGS) -o $(TARGET) do_command.o

do_command.o: do_command.cpp
	$(CXX) $(CXXFLAGS) -c do_command.cpp

clean:
	rm -f $(TARGET) do_command.o
