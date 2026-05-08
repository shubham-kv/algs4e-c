#include "queue_int.h"
#include "common_macros.h"
#include <errno.h>
#include <memory.h>
#include <stdlib.h>

#define ITEM IntQueueItem
#define NODE IntQueueNode
#define QUEUE IntQueue
#define ITER IntQueueIterator

struct IntegerQueueNode {
  IntQueueItem item;
  struct IntegerQueueNode *next;
};

QUEUE IntQueue_Create() {
  QUEUE queue = calloc(1, sizeof(*queue));
  if (IS_NULL(queue)) { errno = ENOMEM; return NULL; }

  IntQueue_Init(queue);
  return queue;
}

inline void IntQueue_Init(QUEUE queue) {
  (queue->first = (queue->last = NULL)), (queue->size = 0);
}

void IntQueue_Clear(QUEUE queue) {
  if (IS_NULL(queue)) { return; }

  while (!IntQueue_IsEmpty(queue)) {
    IntQueue_Dequeue(queue, NULL);
  }
}

void IntQueue_Delete(QUEUE *queue) {
  if (IS_NULL(queue) || IS_NULL(*queue)) { return; }

  IntQueue_Clear(*queue);
  free(*queue), (*queue = NULL);
}

int IntQueue_Enqueue(QUEUE queue, ITEM item) {
  if (IS_NULL(queue)) { errno = EINVAL; return -1; }

  NODE node = calloc(1, sizeof(*node));
  if (IS_NULL(node)) { errno = ENOMEM; return -1; }

  node->item = item;

  if (IS_NOT_NULL(queue->last)) {
    queue->last->next = node;
  } else {
    queue->first = node;
  }

  queue->last = node;
  queue->size++;

  return 0;
}

int IntQueue_Dequeue(QUEUE queue, ITEM *out) {
  if (IS_NULL(queue)) { errno = EINVAL; return -1; }
  if (IS_NULL(queue->first)) { errno = ENODATA; return -1; }

  if (IS_NOT_NULL(out)) {
    *out = queue->first->item;
  }

  NODE next = queue->first->next;
  free(queue->first), (queue->first = NULL);

  if (IS_NULL(next)) {
    queue->last = NULL;
  }
  queue->first = next;
  queue->size--;

  return 0;
}

int IntQueue_Peek(QUEUE queue, ITEM *out) {
  if (IS_NULL(queue)) { errno = EINVAL; return -1; }
  if (IS_NULL(queue->first)) { errno = ENODATA; return -1; }

  if (IS_NOT_NULL(out)) {
    *out = queue->first->item;
  }
  return 0;
}

inline int IntQueue_Size(QUEUE queue) { return queue->size; }

inline bool IntQueue_IsEmpty(QUEUE queue) { return IntQueue_Size(queue) == 0; }

ITER IntQueueIter_Create(QUEUE queue) {
  if (IS_NULL(queue)) { errno = EINVAL; return NULL; }

  ITER iterator = calloc(1, sizeof(*iterator));
  if (IS_NULL(iterator)) { errno = ENOMEM; return NULL; }

  IntQueueIter_Init(iterator, queue);

  return iterator;
}

inline void IntQueueIter_Init(ITER iterator, QUEUE queue) {
  iterator->cur = queue->first;
}

inline void IntQueueIter_Clear(ITER iterator) {
  memset(iterator, 0, sizeof(*iterator));
}

void IntQueueIter_Delete(ITER *iterator) {
  if (IS_NULL(iterator) || IS_NULL(*iterator)) {
    return;
  }
  IntQueueIter_Clear(*iterator);
  free(*iterator), (*iterator = NULL);
}

inline bool IntQueueIter_HasNext(ITER iterator) {
  return IS_NOT_NULL(iterator->cur);
}

int IntQueueIter_GetNext(ITER iterator, ITEM *out) {
  if (IS_NULL(iterator)) { errno = EINVAL; return -1; }
  if (IS_NULL(iterator->cur)) { errno = ENODATA; return -1; }

  *out = iterator->cur->item;
  iterator->cur = iterator->cur->next;
  return 0;
}

#undef ITEM
#undef NODE
#undef QUEUE
#undef ITER
