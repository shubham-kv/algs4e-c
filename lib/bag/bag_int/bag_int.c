#include <errno.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>

#include "bag_int.h"
#include "common_macros.h"
#include "linked_node.h"

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

int IntBagIter_Init(IntBagIterator iterator, IntBag bag) {
  REQUIRE_TRUE(IS_NOT_NULL(bag), EINVAL, EXIT_FAILURE);
  return LinkedNodeIter_Init((LinkedNodeIter)iterator, (LinkedNode)bag->first);
}

IntBagIterator IntBagIter_Create(IntBag bag) {
  REQUIRE_TRUE(IS_NOT_NULL(bag), EINVAL, NULL);
  return (IntBagIterator)LinkedNodeIter_Create((LinkedNode)bag->first);
}

inline int IntBagIter_Clear(IntBagIterator iterator) {
  return LinkedNodeIter_Clear((LinkedNodeIter)iterator);
}

inline int IntBagIter_Delete(IntBagIterator *iterator) {
  return LinkedNodeIter_Delete((LinkedNodeIter *)iterator);
}

inline bool IntBagIter_HasNext(IntBagIterator iterator) {
  return LinkedNodeIter_HasNext((LinkedNodeIter)iterator);
}

int IntBagIter_GetNext(IntBagIterator iterator, IntBagItem *out) {
  REQUIRE_TRUE(IS_NOT_NULL(iterator), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);

  IntBagNode cur;
  const int code =
      LinkedNodeIter_GetNext((LinkedNodeIter)iterator, (LinkedNode *)&cur);

  if (code != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  *out = cur->item;
  return EXIT_SUCCESS;
}
