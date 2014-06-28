SRC_DIR  = src
OBJ_DIR  = obj
TEST_DIR = test

.PHONY: all test setup clean

all:
	@cd $(SRC_DIR) && make

test:
	@cd $(TEST_DIR) && make

setup:
	@mkdir -p $(OBJ_DIR)

clean:
	@rm -rf $(OBJ_DIR)

