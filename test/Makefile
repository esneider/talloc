CFLAGS  = -Wall -Wextra -Werror -g --coverage
BASE    = ..
SRC_DIR = $(BASE)/src
OBJ_DIR = $(BASE)/obj
SOURCES = $(wildcard *.c)
TARGETS = $(addprefix $(OBJ_DIR)/,$(SOURCES:.c=))
TESTS   = $(addsuffix .test,$(TARGETS))

.PHONY: all setup clean

all: setup $(TARGETS) $(TESTS) clean

$(OBJ_DIR)/%: %.c $(OBJ_DIR)/*.o
	$(CC) $(CFLAGS) -I$(SRC_DIR) -o $@ $^

$(OBJ_DIR)/%.test:
	@echo "> In file $*.c"
	@$(@:.test=)

setup:
	@cd $(BASE) && make clean && make

clean:
	@cd $(BASE) && make clean

