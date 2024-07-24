# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c11 -I./tokens -I./lexer -I./parser -I./symboltable

# Target executable
TARGET = xt

# Source files
SRCS = main.c 

# Object files
OBJS = $(SRCS:.c=.o)

# Header files
DEPS = tokens/tokens.h  tokens/tokens_fill.h tokens/tokens_val.h lexer/lexer.h parser/parser.h parser/ast.h parser/exp_stmt.h parser/fun_def.h parser/stmt.h parser/all_stmt.h symboltable/symboltable.h 

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
