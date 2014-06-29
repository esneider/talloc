CFLAGS  = -ansi -pedantic -Wall -Wextra -Werror -g --coverage
BASE    = ..
OBJ_DIR = $(BASE)/obj
SOURCES = $(wildcard *.c)
INCLUDE = $(wildcard *.h)
OBJECTS = $(addprefix $(OBJ_DIR)/,$(SOURCES:.c=.o))

.PHONY: build test setup clean

build: setup $(OBJECTS)

test:
	@make -C $(BASE) test

setup:
	@make -C $(BASE) setup

clean:
	@make -C $(BASE) clean

$(OBJ_DIR)/%.o: %.c $(INCLUDE)
	$(CC) $(CFLAGS) -c -o $@ $<

