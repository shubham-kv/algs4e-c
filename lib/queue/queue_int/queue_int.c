#include "queue_int.h"
#include "common_macros.h"
#include <errno.h>
#include <memory.h>
#include <stdlib.h>

struct IntegerQueueNode {
  IntQueueItem item;
  struct IntegerQueueNode *next;
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

IntQueueIterator IntQueueIter_Create(IntQueue queue) {
  REQUIRE_TRUE(IS_NOT_NULL(queue), EINVAL, NULL);

  IntQueueIterator iterator = calloc(1, sizeof(*iterator));
  REQUIRE_TRUE(IS_NOT_NULL(iterator), ENOMEM, NULL);

  IntQueueIter_Init(iterator, queue);
  return iterator;
}

inline int IntQueueIter_Init(IntQueueIterator iterator, IntQueue queue) {
  REQUIRE_TRUE(IS_NOT_NULL(iterator), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(queue), EINVAL, EXIT_FAILURE);
  iterator->cur = queue->first;
  return EXIT_SUCCESS;
}

inline int IntQueueIter_Clear(IntQueueIterator iterator) {
  REQUIRE_TRUE(IS_NOT_NULL(iterator), EINVAL, EXIT_FAILURE);
  memset(iterator, 0, sizeof(*iterator));
  return EXIT_SUCCESS;
}

int IntQueueIter_Delete(IntQueueIterator *iterator) {
  REQUIRE_TRUE(IS_NOT_NULL(iterator) && IS_NOT_NULL(*iterator), EINVAL,
               EXIT_FAILURE);

  IntQueueIter_Clear(*iterator);
  free(*iterator), (*iterator = NULL);
  return EXIT_SUCCESS;
}

inline bool IntQueueIter_HasNext(IntQueueIterator iterator) {
  return IS_NOT_NULL(iterator->cur);
}

int IntQueueIter_GetNext(IntQueueIterator iterator, IntQueueItem *out) {
  REQUIRE_TRUE(IS_NOT_NULL(iterator), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(iterator->cur), ENODATA, EXIT_FAILURE);
  *out = iterator->cur->item;
  iterator->cur = iterator->cur->next;
  return EXIT_SUCCESS;
}
