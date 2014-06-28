CFLAGS   = -ansi -pedantic -Wall -Wextra -Werror -g --coverage
SRC_DIR  = src
OBJ_DIR  = obj
TEST_DIR = test

.PHONY: talloc test clean

talloc:
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c -o $(OBJ_DIR)/$@.o $(SRC_DIR)/$@.c

test:
	@cd $(TEST_DIR) && make

clean:
	@rm -rf $(OBJ_DIR)

