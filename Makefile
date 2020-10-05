# Compiler and linker.
CC=g++

# Name of the executable.
TARGET=chip8emulator

# Compiler flags.
CCFLAGS=-g -Wall -std=c++17 -I./inc

# List all the sources needed for this project.
SOURCES=$(wildcard src/*.cpp)

# List the objects needed for the project (all come
# from the list in SOURCES).
OBJECTS=$(patsubst %.cpp,%.o,$(SOURCES))

# List dependency files, one for each source file.
DEPENDS=$(patsubst %.cpp,%.d,$(SOURCES))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CCFLAGS) $(OBJECTS) -o $(TARGET)

-include $(DEPENDS)

%.o: %.cpp Makefile
	$(CC) $(CCFLAGS) -MMD -MP -c $< -o $@

clean:
	-rm -f $(OBJECTS) $(DEPENDS) $(TARGET)
