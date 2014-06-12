CFLAGS = -ansi -pedantic -Wall -Wextra -g --coverage

.PHONY: all src test clean

all: src test

src: talloc.c talloc_test.c talloc.h
	$(CC) $(CFLAGS) -o test talloc.c talloc_test.c

test:
	./test

clean:
	rm -f test *.o
