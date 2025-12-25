# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Iinclude

# Source files
SRCS = $(wildcard src/*.c)

# Object files (in build/)
OBJS = $(patsubst src/%.c, build/%.o, $(SRCS))

# Output binary
TARGET = build/img2ascii

# Default rule
all: $(TARGET)

# Link object files into binary
$(TARGET): $(OBJS)
	@mkdir -p build
	$(CC) $(CFLAGS) -o $@ $^

# Compile .c files into .o files
build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -rf build/*.o $(TARGET)

.PHONY: all clean

