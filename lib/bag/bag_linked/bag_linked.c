#include <errno.h>
#include <memory.h>
#include <stdlib.h>

#include "bag_linked.h"
#include "common_macros.h"
#include "linked_node.h"

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

int BagIterator_Init(BagIterator iterator, Bag bag) {
  REQUIRE_TRUE(IS_NOT_NULL(bag), EINVAL, EXIT_FAILURE);
  return LinkedNodeIter_Init((LinkedNodeIter)iterator, (LinkedNode)bag->first);
}

BagIterator BagIterator_Create(Bag bag) {
  REQUIRE_TRUE(IS_NOT_NULL(bag), EINVAL, NULL);
  return (BagIterator)LinkedNodeIter_Create((LinkedNode)bag->first);
}

inline int BagIterator_Clear(BagIterator iterator) {
  return LinkedNodeIter_Clear((LinkedNodeIter)iterator);
}

inline int BagIterator_Delete(BagIterator *iterator) {
  return LinkedNodeIter_Delete((LinkedNodeIter *)iterator);
}

inline bool BagIterator_HasNext(BagIterator iterator) {
  return LinkedNodeIter_HasNext((LinkedNodeIter)iterator);
}

int BagIterator_GetNext(BagIterator iterator, BagItem *out) {
  REQUIRE_TRUE(IS_NOT_NULL(iterator), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);

  BagNode cur;
  const int code =
      LinkedNodeIter_GetNext((LinkedNodeIter)iterator, (LinkedNode *)&cur);

  if (code != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  *out = cur->item;
  return EXIT_SUCCESS;
}
