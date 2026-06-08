# Makefile - Simple Text Editor with Garbage Collection
# CSE 232 Term Project

CC      = gcc
CFLAGS  = -Wall -Wextra -g
LIBS    = -lncurses

SRC = main.c buffer.c file.c insert.c delete_gc.c ui_save.c
OUT = editor

all: $(OUT)

$(OUT): $(SRC) editor.h
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LIBS)

run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT) *.o

.PHONY: all run clean
