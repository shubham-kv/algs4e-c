#include <stdbool.h>
#include <stdlib.h>

#ifndef __queue_linked_h
#define __queue_linked_h

struct QueueNode;
struct LinkedQueue;
struct LinkedQueueIterator;

typedef const void *Item;
typedef struct LinkedQueue *Queue;
typedef struct LinkedQueueIterator *QueueIterator;

struct LinkedQueue {
  struct QueueNode *first;
  struct QueueNode *last;
  size_t size;
};

#define ITEM Item
#define QUEUE Queue
#define ITER QueueIterator

QUEUE Queue_Create();
  int Queue_Init(QUEUE queue);
  int Queue_Clear(QUEUE queue);
  int Queue_Delete(QUEUE *queue);
  int Queue_Enqueue(QUEUE queue, ITEM item);
  int Queue_Dequeue(QUEUE queue, ITEM *out);
  int Queue_Peek(QUEUE queue, ITEM *out);
  int Queue_Size(QUEUE queue);
 bool Queue_IsEmpty(QUEUE queue);

struct LinkedQueueIterator {
  struct QueueNode *cur;
};

ITER QueueIterator_Create(QUEUE queue);
 int QueueIterator_Init(ITER iterator, QUEUE queue);
 int QueueIterator_Clear(ITER iterator);
 int QueueIterator_Delete(ITER *iterator);
bool QueueIterator_HasNext(ITER iterator);
 int QueueIterator_GetNext(ITER iterator, ITEM *out);

#undef ITEM
#undef QUEUE
#undef ITER

#endif // __queue_linked_h
