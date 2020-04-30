SRC_DIR=src
LIB_DIR=$(SRC_DIR)/lib
BIN_DIR=bin
DOC_DIR=doc
EXEC_NAME=main
BUILD_DIR=build
DOC_BUILD=doc

INCLUDES=-I$(LIB_DIR)

.PHONY: test

.PHONY: doc

all: setup doc
	gcc -g $(INCLUDES) $(SRC_DIR)/*.c $(LIB_DIR)/*.c -o $(BIN_DIR)/$(EXEC_NAME)

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
	@rm -rf $(BUILD_DIR)/
	@rm -rf $(DOC_DIR)/
