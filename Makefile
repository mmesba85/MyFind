CC=gcc
EXEC=myfind
SRC= myfind.c mystrlib.c explore_dir.c stack.c parser.c evaluate.c
OBJ= $(SRC:.c=.o)
CFLAGS=-Wall -Werror -Wextra -pedantic -std=c99 -g
VPATH=src


all: $(EXEC)

myfind: $(OBJ)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
	
.PHONY: clean mrproper test

clean:
	rm -rf *.o src/*.o $(EXEC)
