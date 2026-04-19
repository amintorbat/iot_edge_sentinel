CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -O2
SRC = src/main.c
EXEC = sentinel

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(EXEC)

.PHONY: all clean
