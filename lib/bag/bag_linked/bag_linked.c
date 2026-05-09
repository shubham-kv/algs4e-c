#include "bag_linked.h"
#include "common_macros.h"
#include <errno.h>
#include <memory.h>
#include <stdlib.h>

struct LinkedBagNode {
  struct LinkedBagNode *next;
  BagItem item;
};

Bag Bag_Create() {
  Bag bag = calloc(1, sizeof(*bag));
  REQUIRE_TRUE(IS_NOT_NULL(bag), ENOMEM, NULL);
  Bag_Init(bag);
  return bag;
}

int Bag_Init(Bag bag) {
  REQUIRE_TRUE(IS_NOT_NULL(bag), EINVAL, EXIT_FAILURE);
  bag->first = NULL;
  bag->size = 0;
  return EXIT_SUCCESS;
}

int Bag_Clear(Bag bag) {
  REQUIRE_TRUE(IS_NOT_NULL(bag), EINVAL, EXIT_FAILURE);

  while (IS_NOT_NULL(bag->first)) {
    BagNode next = bag->first->next;
    free(bag->first), (bag->first = NULL);
    bag->first = next;
    bag->size--;
  }
  memset(bag, 0, sizeof(*bag));
  return EXIT_SUCCESS;
}

int Bag_Delete(Bag *bag) {
  REQUIRE_TRUE(IS_NOT_NULL(bag) && IS_NOT_NULL(*bag), EINVAL, EXIT_FAILURE);

  Bag_Clear(*bag);
  free(*bag), (*bag = NULL);
  return EXIT_SUCCESS;
}

int Bag_Add(Bag bag, BagItem item) {
  REQUIRE_TRUE(IS_NOT_NULL(bag), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(item), EINVAL, EXIT_FAILURE);

  BagNode node = calloc(1, sizeof(*node));
  REQUIRE_TRUE(IS_NOT_NULL(node), ENOMEM, EXIT_FAILURE);

  node->item = item;
  node->next = bag->first;
  bag->first = node;
  bag->size++;

  return EXIT_SUCCESS;
}

inline int Bag_Size(Bag bag) { return bag->size; }
inline bool Bag_IsEmpty(Bag bag) { return Bag_Size(bag) == 0; }

BagIterator BagIterator_Create(Bag bag) {
  BagIterator iterator = calloc(1, sizeof(*iterator));
  REQUIRE_TRUE(IS_NOT_NULL(iterator), ENOMEM, NULL);
  BagIterator_Init(iterator, bag);
  return iterator;
}

int BagIterator_Init(BagIterator iterator, Bag bag) {
  REQUIRE_TRUE(IS_NOT_NULL(iterator), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(bag), EINVAL, EXIT_FAILURE);
  iterator->cur = bag->first;
  return EXIT_SUCCESS;
}

int BagIterator_Clear(BagIterator iterator) {
  REQUIRE_TRUE(IS_NOT_NULL(iterator), EINVAL, EXIT_FAILURE);
  memset(iterator, 0, sizeof(*iterator));
  return EXIT_SUCCESS;
}

int BagIterator_Delete(BagIterator *iterator) {
  REQUIRE_TRUE(IS_NOT_NULL(iterator) && IS_NOT_NULL(*iterator), EINVAL,
               EXIT_FAILURE);

  BagIterator_Clear(*iterator);
  free(*iterator), (*iterator = NULL);
  return EXIT_SUCCESS;
}

inline bool BagIterator_HasNext(BagIterator iterator) {
  return IS_NOT_NULL(iterator->cur);
}

int BagIterator_GetNext(BagIterator iterator, BagItem *out) {
  REQUIRE_TRUE(IS_NOT_NULL(iterator), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(iterator->cur), ENODATA, EXIT_FAILURE);

  *out = iterator->cur->item;
  iterator->cur = iterator->cur->next;
  return EXIT_SUCCESS;
}
