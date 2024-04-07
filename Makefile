# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c99 -Wno-unused-variable -Wno-incompatible-pointer-types -Wno-argument-type -Wno-unused-parameter

# Source files
SRCS = chash.c hashdb.c rwlocks.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable
TARGET = MyProgram.out

# Build target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJS) $(TARGET)
