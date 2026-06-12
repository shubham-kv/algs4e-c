#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common_macros.h"
#include "queue_linked.h"

#define BUFFER_SIZE 32

/**
 * ### Compile
 * % cmake -S . -B build
 * % make -C build queue_linked_client
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

  char buffer[BUFFER_SIZE];
  Queue queue = Queue_Create();
  if (IS_NULL(queue)) {
    return EXIT_FAILURE;
  }

  while (fscanf(stdin, "%30s", buffer) != EOF) {
    if (strncmp(buffer, "x\0", 2) == 0) {
      break;
    }

    if (strncmp(buffer, "-\0", 2) == 0) {
      if (Queue_IsEmpty(queue)) {
        printf("Queue is empty!\n");
        continue;
      }

      Item item;
      ENSURE_CALL_SUCCESS(Queue_Dequeue(queue, &item));
      printf("dequeue() = '%s'\n", (char *)item);
      free((char *)item), (item = NULL);
      continue;
    }

    size_t bufLen = strlen(buffer);
    char *input = calloc(bufLen + 1, sizeof(*input));
    snprintf(input, bufLen + 1, "%s", buffer);

    ENSURE_CALL_SUCCESS(Queue_Enqueue(queue, input));
    printf("enqueue('%s')\n", input);

    memset(&buffer, 0, BUFFER_SIZE);
  }

  const bool isQueueEmpty = Queue_IsEmpty(queue);

  printf("\n");
  printf("isEmpty() = %s\n", BOOL_TO_STRING(isQueueEmpty));
  printf("size() = %d\n", Queue_Size(queue));

  if (!isQueueEmpty) {
    Item peekedItem;
    ENSURE_CALL_SUCCESS(Queue_Peek(queue, &peekedItem));
    printf("peek() = '%s'\n", (char *)peekedItem);
  }

  QueueIterator iterator = QueueIterator_Create(queue);
  if (IS_NULL(iterator)) {
    return EXIT_FAILURE;
  }

  while (QueueIterator_HasNext(iterator)) {
    Item next;
    ENSURE_CALL_SUCCESS(QueueIterator_GetNext(iterator, &next));
    printf("next() = '%s'\n", (char *)next);
  }
  printf("\n");

  ENSURE_CALL_SUCCESS(QueueIterator_Delete(&iterator));

  while (!Queue_IsEmpty(queue)) {
    Item item;
    ENSURE_CALL_SUCCESS(Queue_Dequeue(queue, &item));
    free((char *)item), (item = NULL);
  }
  ENSURE_CALL_SUCCESS(Queue_Delete(&queue));

  return EXIT_SUCCESS;
}
