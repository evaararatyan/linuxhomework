CXX = g++
CXXFLAGS = -pthread -O2
TARGET = array-summary

all: $(TARGET)

$(TARGET): array-summary.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) array-summary.cpp

clean:
	rm -f $(TARGET)
