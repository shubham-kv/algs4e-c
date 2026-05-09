#include "bag_int.h"
#include "common_macros.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * ### Compile
 * % cmake -S . -B build
 * % make -C build
 *
 * ### Run
 * % echo "1 2 3 4 5 x" > input.txt
 * % ./build/bin/bag_int_client < input.txt
 *
 */
int main() {
  printf("** IntBag **\n");
  printf("============\n");
  printf("\n");

  int input;
  struct IntegerBag _bag, *bag = &_bag;
  ENSURE_CALL_SUCCESS(IntBag_Init(bag));

  while (fscanf(stdin, "%d", &input) != EOF) {
    if (getchar() == 'x') {
      break;
    }

    ENSURE_CALL_SUCCESS(IntBag_Add(bag, input));
    printf("add(%d)\n", input);
  }

  printf("\n");
  printf("is_empty() = %s\n", IntBag_IsEmpty(bag) ? "true" : "false");
  printf("size() = %d\n", IntBag_Size(bag));

  IntBagItem item;
  struct IntegerBagIterator _iterator, *iterator = &_iterator;
  ENSURE_CALL_SUCCESS(IntBagIter_Init(iterator, bag));

  while (IntBagIter_HasNext(iterator)) {
    ENSURE_CALL_SUCCESS(IntBagIter_GetNext(iterator, &item));
    printf("iterator_next() = %lld\n", item);
  }
  printf("\n");

  ENSURE_CALL_SUCCESS(IntBagIter_Clear(iterator));
  iterator = NULL;
  ENSURE_CALL_SUCCESS(IntBag_Clear(bag));
  bag = NULL;

  return 0;
}
