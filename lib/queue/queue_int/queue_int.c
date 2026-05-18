#include <errno.h>
#include <memory.h>
#include <stdlib.h>

#include "common_macros.h"
#include "linked_node.h"
#include "queue_int.h"

struct IntegerQueueNode {
  struct IntegerQueueNode *next;
  IntQueueItem item;
};

IntQueue IntQueue_Create() {
  IntQueue queue = calloc(1, sizeof(*queue));
  REQUIRE_TRUE(IS_NOT_NULL(queue), ENOMEM, NULL);
  IntQueue_Init(queue);
  return queue;
}

int IntQueue_Init(IntQueue queue) {
  REQUIRE_TRUE(IS_NOT_NULL(queue), EINVAL, EXIT_FAILURE);
  queue->first = NULL;
  queue->last = NULL;
  queue->size = 0;
  return EXIT_SUCCESS;
}

int IntQueue_Clear(IntQueue queue) {
  REQUIRE_TRUE(IS_NOT_NULL(queue), EINVAL, EXIT_FAILURE);

  while (!IntQueue_IsEmpty(queue)) {
    IntQueue_Dequeue(queue, NULL);
  }
  memset(queue, 0, sizeof(*queue));
  return EXIT_SUCCESS;
}

int IntQueue_Delete(IntQueue *queue) {
  REQUIRE_TRUE(IS_NOT_NULL(queue) && IS_NOT_NULL(*queue), EINVAL, EXIT_FAILURE);
  IntQueue_Clear(*queue);
  free(*queue), (*queue = NULL);
  return EXIT_SUCCESS;
}

int IntQueue_Enqueue(IntQueue queue, IntQueueItem item) {
  REQUIRE_TRUE(IS_NOT_NULL(queue), EINVAL, EXIT_FAILURE);

  IntQueueNode node = calloc(1, sizeof(*node));
  REQUIRE_TRUE(IS_NOT_NULL(node), ENOMEM, EXIT_FAILURE);
  node->item = item;

  if (IS_NOT_NULL(queue->last)) {
    queue->last->next = node;
  } else {
    queue->first = node;
  }

  queue->last = node;
  queue->size++;

  return EXIT_SUCCESS;
}

int IntQueue_Dequeue(IntQueue queue, IntQueueItem *out) {
  REQUIRE_TRUE(IS_NOT_NULL(queue), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(queue->first), ENODATA, EXIT_FAILURE);

  if (IS_NOT_NULL(out)) {
    *out = queue->first->item;
  }

  IntQueueNode next = queue->first->next;
  free(queue->first), (queue->first = NULL);

  if (IS_NULL(next)) {
    queue->last = NULL;
  }
  queue->first = next;
  queue->size--;

  return EXIT_SUCCESS;
}

int IntQueue_Peek(IntQueue queue, IntQueueItem *out) {
  REQUIRE_TRUE(IS_NOT_NULL(queue), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(queue->first), ENODATA, EXIT_FAILURE);

  if (IS_NOT_NULL(out)) {
    *out = queue->first->item;
  }
  return EXIT_SUCCESS;
}

inline int IntQueue_Size(IntQueue queue) { return queue->size; }

inline bool IntQueue_IsEmpty(IntQueue queue) {
  return IntQueue_Size(queue) == 0;
}

int IntQueueIter_Init(IntQueueIterator iterator, IntQueue queue) {
  REQUIRE_TRUE(IS_NOT_NULL(queue), EINVAL, EXIT_FAILURE);
  return LinkedNodeIter_Init((LinkedNodeIter)iterator,
                             (LinkedNode)queue->first);
}

IntQueueIterator IntQueueIter_Create(IntQueue queue) {
  REQUIRE_TRUE(IS_NOT_NULL(queue), EINVAL, NULL);
  return (IntQueueIterator)LinkedNodeIter_Create((LinkedNode)queue->first);
}

inline int IntQueueIter_Clear(IntQueueIterator iterator) {
  return LinkedNodeIter_Clear((LinkedNodeIter)iterator);
}

inline int IntQueueIter_Delete(IntQueueIterator *iterator) {
  return LinkedNodeIter_Delete((LinkedNodeIter *)iterator);
}

inline bool IntQueueIter_HasNext(IntQueueIterator iterator) {
  return LinkedNodeIter_HasNext((LinkedNodeIter)iterator);
}

int IntQueueIter_GetNext(IntQueueIterator iterator, IntQueueItem *out) {
  REQUIRE_TRUE(IS_NOT_NULL(iterator), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);

  IntQueueNode cur;
  const int code =
      LinkedNodeIter_GetNext((LinkedNodeIter)iterator, (LinkedNode *)&cur);

  if (code != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  *out = cur->item;
  return EXIT_SUCCESS;
}
