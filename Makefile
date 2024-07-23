# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c11

# Target executable
TARGET = clang

# Source files
SRCS = main.c 

# Object files
OBJS = $(SRCS:.c=.o)

# Header files
DEPS = tokens.h lexer.h lex_helper.h parser.h symboltable.h

# Build target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile source files into object files
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target
.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS)
