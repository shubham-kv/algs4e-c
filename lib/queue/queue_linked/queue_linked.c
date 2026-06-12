#include <memory.h>
#include <stdlib.h>

#include "common_macros.h"
#include "linked_node.h"
#include "queue_linked.h"

struct QueueNode {
  struct QueueNode *next;
  Item item;
};

typedef struct QueueNode *Node;

int Queue_Init(Queue queue) {
  REQUIRE_TRUE(IS_NOT_NULL(queue), EINVAL, EXIT_FAILURE);
  queue->first = NULL;
  queue->last = NULL;
  queue->size = 0;
  return EXIT_SUCCESS;
}

Queue Queue_Create() {
  Queue queue = calloc(1, sizeof(*queue));
  REQUIRE_TRUE(IS_NOT_NULL(queue), ENOMEM, NULL);
  Queue_Init(queue);
  return queue;
}

int Queue_Clear(Queue queue) {
  REQUIRE_TRUE(IS_NOT_NULL(queue), EINVAL, EXIT_FAILURE);

  while (!Queue_IsEmpty(queue)) {
    ENSURE_SUCCESS(Queue_Dequeue(queue, NULL));
  }
  memset(queue, 0, sizeof(*queue));
  return EXIT_SUCCESS;
}

int Queue_Delete(Queue *queue) {
  REQUIRE_TRUE(IS_NOT_NULL(queue) && IS_NOT_NULL(*queue), EINVAL, EXIT_FAILURE);
  ENSURE_SUCCESS(Queue_Clear(*queue));
  free(*queue), (*queue = NULL);
  return EXIT_SUCCESS;
}

int Queue_Enqueue(Queue queue, Item item) {
  REQUIRE_TRUE(IS_NOT_NULL(queue), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(item), EINVAL, EXIT_FAILURE);

  Node node = calloc(1, sizeof(*node));
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

int Queue_Dequeue(Queue queue, Item *out) {
  REQUIRE_TRUE(IS_NOT_NULL(queue), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(queue->first), ENODATA, EXIT_FAILURE);

  if (IS_NOT_NULL(out)) {
    *out = queue->first->item;
  }

  Node next = queue->first->next;
  free(queue->first), (queue->first = NULL);

  if (IS_NULL(next)) {
    queue->last = NULL;
  }
  queue->first = next;
  queue->size--;

  return EXIT_SUCCESS;
}

int Queue_Peek(Queue queue, Item *out) {
  REQUIRE_TRUE(IS_NOT_NULL(queue), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(queue->first), ENODATA, EXIT_FAILURE);
  if (IS_NOT_NULL(out)) {
    *out = queue->first->item;
  }
  return EXIT_SUCCESS;
}

inline int Queue_Size(Queue queue) { return queue->size; }

inline bool Queue_IsEmpty(Queue queue) { return Queue_Size(queue) <= 0; }

int QueueIterator_Init(QueueIterator iterator, Queue queue) {
  REQUIRE_TRUE(IS_NOT_NULL(queue), EINVAL, EXIT_FAILURE);
  return LinkedNodeIter_Init((LinkedNodeIter)iterator,
                             (LinkedNode)queue->first);
}

QueueIterator QueueIterator_Create(Queue queue) {
  REQUIRE_TRUE(IS_NOT_NULL(queue), EINVAL, NULL);
  return (QueueIterator)LinkedNodeIter_Create((LinkedNode)queue->first);
}

int QueueIterator_Clear(QueueIterator iterator) {
  return LinkedNodeIter_Clear((LinkedNodeIter)iterator);
}

int QueueIterator_Delete(QueueIterator *iterator) {
  return LinkedNodeIter_Delete((LinkedNodeIter *)iterator);
}

inline bool QueueIterator_HasNext(QueueIterator iterator) {
  return LinkedNodeIter_HasNext((LinkedNodeIter)iterator);
}

int QueueIterator_GetNext(QueueIterator iterator, Item *out) {
  REQUIRE_TRUE(IS_NOT_NULL(iterator), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(out), EINVAL, EXIT_FAILURE);

  Node cur;
  ENSURE_SUCCESS(
      LinkedNodeIter_GetNext((LinkedNodeIter)iterator, (LinkedNode *)&cur));
  *out = cur->item;
  return EXIT_SUCCESS;
}
