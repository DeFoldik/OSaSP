CC = gcc
SRC_DIR = src
OBJ_DIR = obj
BIN = finder
INCLUDE = -Iinclude
LDFLAGS = -lcrypto -lmagic
MAGIC_FILE = magic.mgc

# Default to debug mode
BUILD ?= debug

ifeq ($(BUILD),debug)
	CFLAGS = -g -ggdb -std=c11 -pedantic -W -Wall -Wextra $(INCLUDE) -pthread
else ifeq ($(BUILD),release)
	CFLAGS = -std=c11 -pedantic -W -Wall -Wextra -Werror $(INCLUDE) -pthread
endif

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))

all: $(BIN)

$(BIN): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(BIN)

install:
	cp $(MAGIC_FILE) /usr/local/share/misc/  # Или другое нужное место

.PHONY: clean install all
