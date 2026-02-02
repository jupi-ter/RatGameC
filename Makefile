CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
INCLUDES = -I/usr/local/include
LIBS = -L/usr/local/lib -lraylib -lm -lpthread -ldl -lrt -lX11

SRC_DIR = src
BUILD_DIR = build
TARGET = $(BUILD_DIR)/game

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
