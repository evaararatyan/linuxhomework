CXX := g++
CXXFLAGS := -std=c++17 -pthread -Wall -Wextra
LDFLAGS := -shared
TARGET_LIB := build/libparallel_scheduler.so
TARGET_APP := build/demo-application

all: $(TARGET_LIB) $(TARGET_APP)

$(TARGET_LIB): src/parallel_scheduler.cpp include/parallel_scheduler.h
	$(CXX) $(CXXFLAGS) -Iinclude $(LDFLAGS) -o $@ src/parallel_scheduler.cpp

$(TARGET_APP): src/demo-application.cpp $(TARGET_LIB)
	$(CXX) $(CXXFLAGS) -Iinclude -o $@ src/demo-application.cpp -Lbuild -lparallel_scheduler

clean:
	rm -rf build/*
	mkdir -p build
