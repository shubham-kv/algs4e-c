#include "bag_linked.h"
#include "common_macros.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 32

/**
 * ### Compile
 * % cmake -S . -B build
 * % make -C build
 *
 * ### Run
 * % echo "I II III IV V x" > input.txt
 * % ./build/bin/bag_linked_client < input.txt
 *
 */
int main() {
  printf("** LinkedBag **\n");
  printf("===============\n");
  printf("\n");

  char buffer[BUFFER_SIZE];
  struct LinkedBag _bag, *bag = &_bag;
  ENSURE_CALL_SUCCESS(Bag_Init(bag));

  while (fscanf(stdin, "%31s", buffer) != EOF) {
    if (strncmp(buffer, "x\0", 2) == 0) {
      break;
    } else {
      size_t bufLen = strlen(buffer);
      char *str = calloc(bufLen + 1, sizeof(char));
      snprintf(str, bufLen + 1, "%s", buffer);

      ENSURE_CALL_SUCCESS(Bag_Add(bag, str));
      printf("add(\"%s\")\n", str);
    }
  }

  printf("\n");
  printf("isEmpty() = %s\n", Bag_IsEmpty(bag) ? "true" : "false");
  printf("size() = %d\n", Bag_Size(bag));

  BagItem bagItem;
  struct LinkedBagIterator _iterator, *iterator = &_iterator;
  ENSURE_CALL_SUCCESS(BagIterator_Init(iterator, bag));

  while (BagIterator_HasNext(iterator)) {
    ENSURE_CALL_SUCCESS(BagIterator_GetNext(iterator, &bagItem));
    char *str = (char *)bagItem;
    printf("iterator_next() = \"%s\"\n", str);
    free(str), (str = NULL), (bagItem = NULL);
  }
  printf("\n");

  ENSURE_CALL_SUCCESS(BagIterator_Clear(iterator));
  iterator = NULL;

  ENSURE_CALL_SUCCESS(Bag_Clear(bag));
  bag = NULL;

  return EXIT_SUCCESS;
}

#undef BUFFER_SIZE
