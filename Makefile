CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

SRC = main.c header.c
OBJ = $(SRC:.c=.o)
EXEC = game_of_life

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ)

%.o: %.c header.h
	$(CC) $(CFLAGS) -c $< -o $@

run: $(EXEC)
	./$(EXEC)

clean:
	rm -f *.o $(EXEC) output.txt

.PHONY: all run clean
