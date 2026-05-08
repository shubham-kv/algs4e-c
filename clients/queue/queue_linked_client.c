#include "queue_linked.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 128

/**
 * ### Compile
 * % cmake -S . -B build
 * % make -C build
 *
 * ### Run
 * % echo "I II III IV V - - x" > input.txt
 * % ./build/bin/queue_linked_client < input.txt
 *
 */
int main() {
  printf("** LinkedQueue **\n");
  printf("=================\n");
  printf("\n");

  char inputBuffer[BUFFER_SIZE];
  struct LinkedQueue _queue, *queue = &_queue;
  Queue_Init(queue);

  while (fscanf(stdin, "%127s", inputBuffer) != EOF) {
    if (strncmp(inputBuffer, "x\0", 2) == 0) {
      break;
    }

    if (strncmp(inputBuffer, "-\0", 2) == 0) {
      if (Queue_IsEmpty(queue)) {
        printf("Queue is empty!\n");
        continue;
      }

      char *str = (char *)Queue_Dequeue(queue);
      printf("dequeue() = %s\n", str);
      free(str), (str = NULL);
      continue;
    }

    size_t bufLen = strlen(inputBuffer);
    char *input = calloc(bufLen + 1, sizeof(*input));
    snprintf(input, bufLen + 1, "%s", inputBuffer);

    printf("enqueue(%s)\n", input);
    Queue_Enqueue(queue, input);
  }

  printf("\n");
  printf("isEmpty() = %s\n", Queue_IsEmpty(queue) ? "true" : "false");
  printf("size() = %d\n", Queue_Size(queue));
  printf("peek() = %s\n", (char *)Queue_Peek(queue));

  struct LinkedQueueIterator _iterator, *iterator = &_iterator;
  QueueIterator_Init(iterator, queue);

  while (QueueIterator_HasNext(iterator)) {
    const char *str = (char *)QueueIterator_GetNext(iterator);
    printf("iterator_next() = %s\n", str);
  }
  printf("\n");

  QueueIterator_Clear(iterator), (iterator = NULL);

  while (!Queue_IsEmpty(queue)) {
    free((char *)Queue_Dequeue(queue));
  }
  Queue_Clear(queue), (queue = NULL);

  return 0;
}
