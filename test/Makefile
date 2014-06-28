CFLAGS  = -Wall -Wextra -Werror -g --coverage
BASE    = ..
SRC_DIR = $(BASE)/src
OBJ_DIR = $(BASE)/obj
SOURCES = $(wildcard *.c)

all:
	@cd $(BASE) && make clean && make
	@make $(SOURCES:.c=.build)
	@make $(SOURCES:.c=.run)
	@cd $(BASE) && make clean

%.build:
	$(CC) $(CFLAGS) -I$(SRC_DIR) -o $(OBJ_DIR)/$* $(OBJ_DIR)/*.o $*.c

%.run:
	@echo "> In file $*.c"
	@$(OBJ_DIR)/$*

