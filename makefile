CC = gcc
CFLAGS = -Wall -Wextra -g3
SRC_DIR = source
BUILD_DIR = build
OUTPUT = labirintite

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	$(CC) $(OBJS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)/*.o $(OUTPUT)

.PHONY: all clean
