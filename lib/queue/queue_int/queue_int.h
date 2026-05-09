#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifndef __queue_int_h
#define __queue_int_h

struct IntegerQueueNode;
struct IntegerQueue;
struct IntegerQueueIterator;

typedef int64_t IntQueueItem;
typedef struct IntegerQueueNode *IntQueueNode;
typedef struct IntegerQueue *IntQueue;
typedef struct IntegerQueueIterator *IntQueueIterator;

#define ITEM IntQueueItem
#define QUEUE IntQueue
#define ITER IntQueueIterator

struct IntegerQueue {
  struct IntegerQueueNode *first;
  struct IntegerQueueNode *last;
  size_t size;
};

QUEUE IntQueue_Create();
  int IntQueue_Init(QUEUE queue);
  int IntQueue_Clear(QUEUE queue);
  int IntQueue_Delete(QUEUE *queue);
  int IntQueue_Enqueue(QUEUE queue, ITEM item);
  int IntQueue_Dequeue(QUEUE queue, ITEM *out);
  int IntQueue_Peek(QUEUE queue, ITEM *out);
  int IntQueue_Size(QUEUE queue);
 bool IntQueue_IsEmpty(QUEUE queue);

struct IntegerQueueIterator {
  struct IntegerQueueNode *cur;
};

ITER IntQueueIter_Create(QUEUE queue);
 int IntQueueIter_Init(ITER iterator, QUEUE queue);
 int IntQueueIter_Clear(ITER iterator);
 int IntQueueIter_Delete(ITER *iterator);
bool IntQueueIter_HasNext(ITER iterator);
 int IntQueueIter_GetNext(ITER iterator, ITEM *out);

#undef ITEM
#undef QUEUE
#undef ITER

#endif // __queue_int_h
