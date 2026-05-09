#include "common_macros.h"
#include "queue_int.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 64

/**
 * ### Compile
 * % cmake -S . -B build
 * % make -C build
 *
 * ### Run
 * % echo "1 2 3 4 - - 5 6 - x" > input.txt
 * % ./build/bin/queue_int_client < input.txt
 *
 */
int main() {
  printf("** IntQueue **\n");
  printf("==============\n");
  printf("\n");

  char buffer[BUFFER_SIZE];
  struct IntegerQueue _queue, *queue = &_queue;
  IntQueue_Init(queue);

  while (fscanf(stdin, "%63s", buffer) != EOF) {
    if (strncmp(buffer, "x\0", 2) == 0) {
      break;
    } else if (strncmp(buffer, "-\0", 2) == 0) {
      if (IntQueue_IsEmpty(queue)) {
        printf("Queue is empty!\n");
        continue;
      }

      IntQueueItem integer;
      const int code = IntQueue_Dequeue(queue, &integer);
      if (code != SUCCESS) { return code; }
      printf("dequeue() = %lld\n", integer);

    } else {
      char *end;
      IntQueueItem integer = (IntQueueItem)strtol(buffer, &end, 10);
      if (end == buffer) {
        fprintf(stderr, "Invalid input!\n");
        return ERROR;
      }

      const int code = IntQueue_Enqueue(queue, integer);
      if (code != SUCCESS) { return code; }
      printf("enqueue(%lld)\n", integer);
    }

    memset(&buffer, 0, BUFFER_SIZE);
  }

  int code;
  IntQueueItem integer;
  bool isQueueEmpty = IntQueue_IsEmpty(queue);

  printf("\n");
  printf("isEmpty() = %s\n", isQueueEmpty ? "true" : "false");
  printf("size() = %d\n", IntQueue_Size(queue));

  if (!isQueueEmpty) {
    code = IntQueue_Peek(queue, &integer);
    if (code != SUCCESS) { return code; }
    printf("peek() = %lld\n", integer);
  }

  struct IntegerQueueIterator _iterator, *iterator = &_iterator;
  IntQueueIter_Init(iterator, queue);

  while (IntQueueIter_HasNext(iterator)) {
    code = IntQueueIter_GetNext(iterator, &integer);
    if (code != SUCCESS) { return code; }
    printf("iterator_next() = %lld\n", integer);
  }
  printf("\n");

  IntQueueIter_Clear(iterator);
  IntQueue_Clear(queue), (queue = NULL);

  return SUCCESS;
}
