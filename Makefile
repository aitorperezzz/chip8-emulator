# Compiler and linker.
CC=g++

# Name of the executable.
TARGET=chip8
TEST_TARGET=test/runtest

# Compiler flags.
CCFLAGS=-g -Wall -std=c++17 -I./inc
TEST_CCFLAGS=-g -Wall -std=c++17 -I./inc

# Linking flags for unit tests
LDFLAGS=-lcppunit

# List all the sources needed for this project.
SOURCES=$(wildcard src/*.cpp)
MAIN_SOURCES=$(SOURCES) main.cpp
TEST_SOURCES=$(wildcard test/*.cpp)

# List the objects needed for the project (all come
# from the list in SOURCES).
OBJECTS=$(patsubst %.cpp,%.o,$(SOURCES))
MAIN_OBJECTS=$(patsubst %.cpp,%.o,$(MAIN_SOURCES))
TEST_OBJECTS=$(patsubst test/%.cpp,test/%.o,$(TEST_SOURCES))

# List dependency files, one for each source file.
DEPENDS=$(patsubst %.cpp,%.d,$(SOURCES))
MAIN_DEPENDS=$(patsubst %.cpp,%.d,$(MAIN_SOURCES))
TEST_DEPENDS=$(patsubst test/%.cpp,test/%.d,$(TEST_SOURCES)) $(patsubst %.cpp,%.d,$(SOURCES))

.PHONY: all clean check

all: $(TARGET)

check: $(TEST_TARGET)
	./test/runtest

$(TARGET): $(MAIN_OBJECTS)
	$(CC) $(CCFLAGS) $(MAIN_OBJECTS) -o $(TARGET)

-include $(MAIN_DEPENDS)

$(TEST_TARGET): $(OBJECTS) $(TEST_OBJECTS)
	$(CC) $(TEST_CCFLAGS) $(OBJECTS) $(TEST_OBJECTS) -o $(TEST_TARGET) $(LDFLAGS)

-include $(DEPENDS)

%.o: %.cpp Makefile
	$(CC) $(CCFLAGS) -MMD -MP -c $< -o $@

test/%.o: test/%.cpp Makefile
	$(CC) $(TEST_CCFLAGS) -MMD -MP -c $< -o $@

clean:
	-rm -f $(OBJECTS) $(DEPENDS) $(TARGET)
	-rm -f $(MAIN_OBJECTS) $(MAIN_DEPENDS) $(TARGET)
	-rm -f $(TEST_OBJECTS) $(TEST_DEPENDS) $(TEST_TARGET)
