# Detect OS and architecture
# Windows
ifeq ($(OS),Windows_NT)
	PLATFORM = win
	ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
		ARCH = x86_64
	else
		# x86_64
		ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
			ARCH = x86_64
		endif
		# x86
		ifeq ($(PROCESSOR_ARCHITECTURE),x86)
			ARCH = x86
		endif
	endif
# Not Windows
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		PLATFORM = linux
	endif
	ifeq ($(UNAME_S),Darwin)
		PLATFORM = macos
	endif
	UNAME_P := $(shell uname -p)
	ifeq ($(UNAME_P),x86_64)
		ARCH = x86_64
	endif
	ifneq ($(filter %86,$(UNAME_P)),)
		ARCH = x86
	endif
	ifneq ($(filter arm%,$(UNAME_P)),)
		ARCH = arm
	endif
endif

# Set compiler and flags based on OS
# Linux
ifeq ($(PLATFORM),linux)
	CXX = clang++
	CXXFLAGS = -Wall -Os -std=c++11 -faddrsig -flto -fdata-sections -ffunction-sections
	LDFLAGS = -fuse-ld=lld -Wl,-icf=safe -Wl,-s -Wl,--gc-sections
endif
# macOS
ifeq ($(PLATFORM),macos)
	CXX = clang++
	CXXFLAGS = -Wall -Os -std=c++11 -faddrsig -flto -fdata-sections -ffunction-sections
endif

# Target executable
TARGET = wol

# Default installation directory
DESTDIR = /usr/local/bin

# Source files
SRCS = wol.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Define certain rules as phony
.PHONY: install

# Default rule to build the executable
all: $(TARGET)

# Rule to link object files into the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to install executable
install: all
	sudo cp $(TARGET) $(DESTDIR)

# Rule to remove generated files
clean:
	$(RM) $(TARGET) $(OBJS)
