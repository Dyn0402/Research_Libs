# Compiler and ROOT flags
CXX = g++
ROOTCFLAGS = $(shell /star/u/dneff/Software/root/root-6.14.06/obj/bin/root-config --cflags)
ROOTLIBS = $(shell /star/u/dneff/Software/root/root-6.14.06/obj/bin/root-config --libs)
CXXFLAGS = -Wall -O2 -std=c++11 $(ROOTCFLAGS)

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
