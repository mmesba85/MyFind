CC=gcc
EXEC=myfind
SRC= myfind.c mystrlib.c stack.c parser.c evaluate.c test.c action.c
OBJ= $(SRC:.c=.o)
CFLAGS=-Wall -Werror -Wextra -pedantic -std=c99 -g
VPATH=src


all: $(EXEC)

myfind: $(OBJ)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

doc:
	doxygen config_doc

test:
	tests/test.sh
	
.PHONY: clean mrproper test

clean:
	rm -rf *.o src/*.o $(EXEC)
