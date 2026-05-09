#include "bag_int.h"
#include "common_macros.h"
#include <errno.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>

struct IntegerBagNode {
  struct IntegerBagNode *next;
  IntBagItem item;
};

IntBag IntBag_Create() {
  IntBag bag = calloc(1, sizeof(*bag));
  REQUIRE_TRUE(IS_NOT_NULL(bag), ENOMEM, NULL);
  IntBag_Init(bag);
  return bag;
}

int IntBag_Init(IntBag bag) {
  REQUIRE_TRUE(IS_NOT_NULL(bag), EINVAL, EXIT_FAILURE);
  bag->first = NULL;
  bag->size = 0;
  return EXIT_SUCCESS;
}

int IntBag_Clear(IntBag bag) {
  REQUIRE_TRUE(IS_NOT_NULL(bag), EINVAL, EXIT_FAILURE);

  while (IS_NOT_NULL(bag->first)) {
    IntBagNode next = bag->first->next;
    free(bag->first), (bag->first = NULL);
    bag->first = next;
    bag->size--;
  }
  memset(bag, 0, sizeof(*bag));
  return EXIT_SUCCESS;
}

int IntBag_Delete(IntBag *bag) {
  REQUIRE_TRUE(IS_NOT_NULL(bag) && IS_NOT_NULL(*bag), EINVAL, EXIT_FAILURE);

  IntBag_Clear(*bag);
  free(*bag), (*bag = NULL);
  return EXIT_SUCCESS;
}

int IntBag_Add(IntBag bag, IntBagItem item) {
  REQUIRE_TRUE(IS_NOT_NULL(bag), EINVAL, EXIT_FAILURE);

  IntBagNode node = calloc(1, sizeof(*node));
  REQUIRE_TRUE(IS_NOT_NULL(node), ENOMEM, EXIT_FAILURE);

  node->item = item;
  node->next = bag->first;
  bag->first = node;
  bag->size++;

  return EXIT_SUCCESS;
}

inline int IntBag_Size(IntBag bag) { return bag->size; }
inline bool IntBag_IsEmpty(IntBag bag) { return IntBag_Size(bag) == 0; }

IntBagIterator IntBagIter_Create(IntBag bag) {
  IntBagIterator iterator = calloc(1, sizeof(*iterator));
  REQUIRE_TRUE(IS_NOT_NULL(iterator), ENOMEM, NULL);
  IntBagIter_Init(iterator, bag);
  return iterator;
}

int IntBagIter_Init(IntBagIterator iterator, IntBag bag) {
  REQUIRE_TRUE(IS_NOT_NULL(iterator), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(bag), EINVAL, EXIT_FAILURE);
  iterator->cur = bag->first;
  return EXIT_SUCCESS;
}

int IntBagIter_Clear(IntBagIterator iterator) {
  REQUIRE_TRUE(IS_NOT_NULL(iterator), EINVAL, EXIT_FAILURE);
  memset(iterator, 0, sizeof(*iterator));
  return EXIT_SUCCESS;
}

int IntBagIter_Delete(IntBagIterator *iterator) {
  REQUIRE_TRUE(IS_NOT_NULL(iterator) && IS_NOT_NULL(*iterator), EINVAL,
               EXIT_FAILURE);

  IntBagIter_Clear(*iterator);
  free(*iterator), (*iterator = NULL);
  return EXIT_SUCCESS;
}

inline bool IntBagIter_HasNext(IntBagIterator iterator) {
  return IS_NOT_NULL(iterator->cur);
}

int IntBagIter_GetNext(IntBagIterator iterator, IntBagItem *out) {
  REQUIRE_TRUE(IS_NOT_NULL(iterator), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(iterator->cur), ENODATA, EXIT_FAILURE);

  *out = iterator->cur->item;
  iterator->cur = iterator->cur->next;
  return EXIT_SUCCESS;
}
