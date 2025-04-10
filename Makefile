CC = gcc
CFLAGS = -Wall -Wextra -std=c99

SRC = main.c header.c
OBJ = $(SRC:.c=.o)
EXEC = program.exe

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c header.h
	$(CC) $(CFLAGS) -c $< -o $@

run: $(EXEC)
	./$(EXEC) InputData/input.txt output/output.txt

clean:
	del /Q *.o *.exe 2>nul || rm -f *.o *.exe

.PHONY: all clean run
