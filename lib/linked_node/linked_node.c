#include <errno.h>
#include <memory.h>
#include <stdlib.h>

#include "common_macros.h"
#include "linked_node.h"

int LinkedNodeIter_Init(LinkedNodeIter iterator, LinkedNode node) {
  REQUIRE_TRUE(IS_NOT_NULL(iterator), EINVAL, EXIT_FAILURE);
  iterator->cur = node;
  return EXIT_SUCCESS;
}

LinkedNodeIter LinkedNodeIter_Create(LinkedNode node) {
  LinkedNodeIter iterator = calloc(1, sizeof(*iterator));
  REQUIRE_TRUE(IS_NOT_NULL(iterator), ENOMEM, NULL);
  LinkedNodeIter_Init(iterator, node);
  return iterator;
}

int LinkedNodeIter_Clear(LinkedNodeIter iterator) {
  REQUIRE_TRUE(IS_NOT_NULL(iterator), EINVAL, EXIT_FAILURE);
  memset(iterator, 0, sizeof(*iterator));
  return EXIT_SUCCESS;
}

int LinkedNodeIter_Delete(LinkedNodeIter *iterator) {
  REQUIRE_TRUE(IS_NOT_NULL(iterator) && IS_NOT_NULL(*iterator), EINVAL,
               EXIT_FAILURE);
  LinkedNodeIter_Clear(*iterator);
  free(*iterator), (*iterator = NULL);
  return EXIT_SUCCESS;
}

inline bool LinkedNodeIter_HasNext(LinkedNodeIter iterator) {
  return IS_NOT_NULL(iterator->cur);
}

int LinkedNodeIter_GetNext(LinkedNodeIter iterator, LinkedNode *out) {
  REQUIRE_TRUE(IS_NOT_NULL(iterator), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(iterator->cur), ENODATA, EXIT_FAILURE);
  *out = iterator->cur;
  iterator->cur = iterator->cur->next;
  return EXIT_SUCCESS;
}
