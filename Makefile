SRC_DIR  = src
OBJ_DIR  = obj
TEST_DIR = test

.PHONY: build test setup clean

build:
	@make -C $(SRC_DIR)

test:
	@make -C $(TEST_DIR)

setup:
	@mkdir -p $(OBJ_DIR)

clean:
	@rm -rf $(OBJ_DIR)

