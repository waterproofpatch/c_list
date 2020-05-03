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

# Everything
all: driver setup doc

# The driver program
driver: list
	gcc -g $(INCLUDES) $(SRC_DIR)/main.c -L$(BIN_DIR) -llist -o $(BIN_DIR)/$(EXEC_NAME)

# The list static library
list:
	gcc -c -I$(LIB_DIR) $(LIB_DIR)/*.c -o $(BIN_DIR)/list.o
	ar -rc $(BIN_DIR)/liblist.a $(BIN_DIR)/list.o

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
