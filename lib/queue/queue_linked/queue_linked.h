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

void Queue_Init(Queue queue);
void Queue_Clear(Queue queue);
void Queue_Enqueue(Queue queue, Item item);
Item Queue_Dequeue(Queue queue);
Item Queue_Peek(Queue queue);
 int Queue_Size(Queue queue);
bool Queue_IsEmpty(Queue queue);

struct LinkedQueueIterator {
  struct QueueNode *cur;
};

void QueueIterator_Init(QueueIterator iterator, Queue queue);
void QueueIterator_Clear(QueueIterator iterator);
bool QueueIterator_HasNext(QueueIterator iterator);
Item QueueIterator_GetNext(QueueIterator iterator);

#endif // __queue_linked_h
