SRC_DIR=src
LIB_DIR=$(SRC_DIR)/lib
BIN_DIR=bin
DOC_DIR=doc
EXEC_NAME=main
BUILD_DIR=build
DOC_BUILD=doc

CFLAGS=-Wall -Werror
CC=gcc

INCLUDES=-I$(LIB_DIR)

.PHONY: test
.PHONY: doc
.PHONY: dirs
# Everything
all: dirs driver setup doc


# Create the output directories
dirs: 
	mkdir -p $(BIN_DIR)
	mkdir -p $(BUILD_DIR)

# The driver program
driver: dirs lib
	$(CC) -g $(CFLAGS) $(INCLUDES) $(SRC_DIR)/main.c -L$(BIN_DIR) -llist -o $(BIN_DIR)/$(EXEC_NAME)

# The list static library
lib: dirs
	$(CC) -c $(CFLAGS) -I$(LIB_DIR) $(LIB_DIR)/*.c -o $(BIN_DIR)/list.o
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
