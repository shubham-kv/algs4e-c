#include "queue_linked.h"
#include "common_macros.h"
#include <memory.h>
#include <stdlib.h>

struct QueueNode {
  struct QueueNode *next;
  Item item;
};

typedef struct QueueNode *Node;

inline void Queue_Init(Queue queue) {
  (queue->first = (queue->last = NULL)), (queue->size = 0);
}

void Queue_Clear(Queue queue) {
  while (!Queue_IsEmpty(queue)) {
    Queue_Dequeue(queue);
  }
  memset(queue, 0, sizeof(*queue));
}

void Queue_Enqueue(Queue queue, Item item) {
  Node node = calloc(1, sizeof(*node));
  node->item = item;

  if (IS_NOT_NULL(queue->last)) {
    queue->last->next = node;
  } else {
    queue->first = node;
  }

  queue->last = node;
  queue->size++;
}

Item Queue_Dequeue(Queue queue) {
  if (IS_NULL(queue->first)) {
    return NULL;
  }

  Item item = queue->first->item;
  Node next = queue->first->next;
  free(queue->first), (queue->first = NULL);

  if (IS_NULL(next)) {
    queue->last = NULL;
  }
  queue->first = next;
  queue->size--;

  return item;
}

inline Item Queue_Peek(Queue queue) {
  return IS_NULL(queue->first) ? NULL : queue->first->item;
}

inline int Queue_Size(Queue queue) { return queue->size; }

inline bool Queue_IsEmpty(Queue queue) { return Queue_Size(queue) == 0; }

inline void QueueIterator_Init(QueueIterator iterator, Queue queue) {
  iterator->cur = queue->first;
}

inline void QueueIterator_Clear(QueueIterator iterator) {
  memset(iterator, 0, sizeof(*iterator));
}

inline bool QueueIterator_HasNext(QueueIterator iterator) {
  return IS_NOT_NULL(iterator->cur);
}

Item QueueIterator_GetNext(QueueIterator iterator) {
  if (IS_NULL(iterator->cur)) {
    return NULL;
  }
  Item item = iterator->cur->item;
  iterator->cur = iterator->cur->next;
  return item;
}
