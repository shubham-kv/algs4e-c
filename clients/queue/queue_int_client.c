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
  ENSURE_CALL_SUCCESS(IntQueue_Init(queue));

  while (fscanf(stdin, "%63s", buffer) != EOF) {
    if (strncmp(buffer, "x\0", 2) == 0) {
      break;
    } else if (strncmp(buffer, "-\0", 2) == 0) {
      if (IntQueue_IsEmpty(queue)) {
        printf("Queue is empty!\n");
        continue;
      }

      IntQueueItem integer;
      ENSURE_CALL_SUCCESS(IntQueue_Dequeue(queue, &integer));
      printf("dequeue() = %lld\n", integer);

    } else {
      char *end;
      IntQueueItem integer = (IntQueueItem)strtol(buffer, &end, 10);
      if (end == buffer) {
        fprintf(stderr, "Invalid input!\n");
        return EXIT_FAILURE;
      }

      ENSURE_CALL_SUCCESS(IntQueue_Enqueue(queue, integer));
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
    ENSURE_CALL_SUCCESS(IntQueue_Peek(queue, &integer));
    printf("peek() = %lld\n", integer);
  }

  struct IntegerQueueIterator _iterator, *iterator = &_iterator;
  ENSURE_CALL_SUCCESS(IntQueueIter_Init(iterator, queue));

  while (IntQueueIter_HasNext(iterator)) {
    ENSURE_CALL_SUCCESS(IntQueueIter_GetNext(iterator, &integer));
    printf("iterator_next() = %lld\n", integer);
  }
  printf("\n");

  ENSURE_CALL_SUCCESS(IntQueueIter_Clear(iterator));
  ENSURE_CALL_SUCCESS(IntQueue_Clear(queue));
  queue = NULL;

  return EXIT_SUCCESS;
}
