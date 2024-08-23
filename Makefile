CC = gcc
CFLAGS = -Wall -Wextra $(addprefix -I,$(shell find include -type d))
SRC_DIR = src
BUILD_DIR = build
TEST_DIR = test
SOURCES = $(wildcard $(SRC_DIR)/*/*.c) main.c
OBJECTS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(SOURCES))
TARGET = out
EXECUTABLE = xt


all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	find $(BUILD_DIR) -name '*.o' -delete
	rm -f $(EXECUTABLE)

test: $(EXECUTABLE)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/test_a $(TEST_DIR)/test_a.c -L$(BUILD_DIR) -lprogram
	$(BUILD_DIR)/test_a

.PHONY: all clean test
