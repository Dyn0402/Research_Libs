# Compiler and ROOT flags
CXX = g++
ROOTCFLAGS = $(shell /local/home/dn277127/Software/root/bin/root-config --cflags)
ROOTLIBS = $(shell /local/home/dn277127/Software/root/bin/root-config --libs)
CXXFLAGS = -Wall -O2 -std=c++17 $(ROOTCFLAGS)

# Sources and objects
SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)
TARGET = libfluctuation.a

# Default rule
all: $(TARGET)

$(TARGET): $(OBJ)
	ar rcs $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET)
