SRC_DIR=src
BIN_DIR=bin
DOC_DIR=doc
EXEC_NAME=main

.PHONY: test
	test

.PHONY: doc
	doc

all: setup doc
	gcc -g $(SRC_DIR)/*.c -o $(BIN_DIR)/$(EXEC_NAME)

doc:
	@doxygen Doxyfile 

run: all
	$(BIN_DIR)/$(EXEC_NAME)


setup:
	@mkdir -p $(BIN_DIR)

test: setup
	@mkdir -p test/support
	@ceedling clobber
	@ceedling gcov:all
	@ceedling utils:gcov

clean:
	@rm -rf $(BIN_DIR)/
	@rm -rf $(DOC_DIR)/
