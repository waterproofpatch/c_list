# Linked List in C
Simple linked list implementation in C (using only C std lib). CMock tests are included.


## Example Usage
from src/example1.c
```c
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "example1.h"
#include "list.h"

void item_processor(void* item, void* context);

void example1_run(void)
{
    // item to add to list
    char* mystring1 = (char*)malloc(strlen("this is my string1") + 1);
    char* mystring2 = (char*)malloc(strlen("this is my string2") + 1);
    strcpy(mystring1, "this is my string1");
    strcpy(mystring2, "this is my string2");

    // create a list
    list_t* list = list_init(malloc, free);

    // add our item
    list_add(list, mystring1);
    list_add(list, mystring2);

    // process each element
    list_foreach(list, item_processor, "pass me");

    list_destroy(list);
}

void item_processor(void* item, void* context)
{
    printf("got item: '%s', context '%s'\n", item, context);
}
```

## Prerequisites

* doxygen
* ruby
* ceedling
* gcovr

## Build

### Documentation

```bash
make doc
```

### Example

```bash
make
```

### Tests

```bash
make test
```

## Run Example

```bash
make run
```

## Run Tests

This assumes you have ruby/ceedling

```bash
make test
```

## License
This project is released under the MIT license - see https://opensource.org/licenses/MIT for details.
