CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS =

SRC_DIR = src
OBJ_DIR = build
BIN_DIR = bin
EXAMPLES_DIR = examples
INCLUDE_DIR = include

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
EXAMPLES = $(wildcard $(EXAMPLES_DIR)/*.c)
EXAMPLE_BINS = $(EXAMPLES:$(EXAMPLES_DIR)/%.c=$(BIN_DIR)/%)

.PHONY: all clean examples

all: examples

examples: $(EXAMPLE_BINS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/%: $(EXAMPLES_DIR)/%.c $(OBJECTS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $< $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
