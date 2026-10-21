# Makefile for BSD/Linux/macOS

# Compiler
CXX = clang++

# Compiler flags
CXXFLAGS = -Wall -Os -std=c++23

# Define certain rules as phony
.PHONY: install

# Target executable
TARGET = wol

# Default installation directory
DESTDIR = /usr/local/bin

# Source files
SRCS = wol.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default rule to build the executable
all: $(TARGET)

# Rule to link object files into the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to install executable
install:
	sudo cp $(TARGET) $(DESTDIR)

# Rule to remove generated files
clean:
	$(RM) $(TARGET) $(OBJS)
