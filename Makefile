# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -O0
LDFLAGS = -lSDL2 -lSDL2_ttf -lSDL2_image -lm -lpthread

# Directories
SRC_DIR = src
LIB_DIR = lib
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj

# Target executable
TARGET = cimple_ui

# Source files
MAIN_SRC = $(SRC_DIR)/main.c
LIB_SRCS = $(LIB_DIR)/arena_memory/arena_memory.c \
           $(LIB_DIR)/Cimple_ui/Cimple_ui.c \
           $(LIB_DIR)/string_lib/string.c

# Object files
MAIN_OBJ = $(OBJ_DIR)/main.o
LIB_OBJS = $(OBJ_DIR)/arena_memory.o \
           $(OBJ_DIR)/Cimple_ui.o \
           $(OBJ_DIR)/string.o

ALL_OBJS = $(MAIN_OBJ) $(LIB_OBJS)

# Default target
all: $(TARGET)

# Create build directories
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(OBJ_DIR): $(BUILD_DIR)
	mkdir -p $(OBJ_DIR)

# Build target executable
$(TARGET): $(ALL_OBJS)	
	$(CC) $(ALL_OBJS) -o $@ $(LDFLAGS)


# Build main object file
$(OBJ_DIR)/main.o: $(MAIN_SRC) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Build library object files
$(OBJ_DIR)/arena_memory.o: $(LIB_DIR)/arena_memory/arena_memory.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/Cimple_ui.o: $(LIB_DIR)/Cimple_ui/Cimple_ui.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/string.o: $(LIB_DIR)/string_lib/string.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Debug with gdb
debug: $(TARGET)
	gdb ./$(TARGET)

# Install dependencies (Ubuntu/Debian)
install-deps:
	sudo apt-get update
	sudo apt-get install libsdl2-dev libsdl2-ttf-dev build-essential gdb

# Check for memory leaks with valgrind
valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

# Show help
help:
	@echo "Available targets:"
	@echo "  all          - Build the project (default)"
	@echo "  clean        - Remove build files"
	@echo "  run          - Build and run the program"
	@echo "  debug        - Build and run with gdb"
	@echo "  valgrind     - Build and run with valgrind memory check"
	@echo "  install-deps - Install required dependencies (Ubuntu/Debian)"
	@echo "  help         - Show this help message"

# Phony targets
.PHONY: all clean run debug install-deps valgrind help
