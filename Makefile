# Makefile for Simplified-DES-CS4770

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -g

# Target executable
TARGET = decrypter

# Source files
SRCS = main.cc

# Object files
OBJS = $(SRCS:.cc=.o)

# Default target
all: $(TARGET)

# Link the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile source files into object files
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)