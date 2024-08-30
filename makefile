CC = gcc
CFLAGS = -Wall -Wextra

SRC_DIR = .
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

OBJ_FILES = $(SRC_FILES:.c=.o)

OUTPUT = vectorsum

all: $(OUTPUT)

$(OUTPUT): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $(OBJ_FILES) -lm

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJ_FILES)

.PHONY: all clean
