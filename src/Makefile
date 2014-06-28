CFLAGS  = -ansi -pedantic -Wall -Wextra -Werror -g --coverage
BASE    = ..
OBJ_DIR = $(BASE)/obj
SOURCES = $(wildcard *.c)
INCLUDE = $(wildcard *.h)
OBJECTS = $(addprefix $(OBJ_DIR)/,$(SOURCES:.c=.o))

.PHONY: all setup clean

all: setup $(OBJECTS)

$(OBJ_DIR)/%.o: %.c $(INCLUDE)
	$(CC) $(CFLAGS) -c -o $@ $<

setup:
	@cd $(BASE) && make setup

clean:
	@cd $(BASE) && make clean
