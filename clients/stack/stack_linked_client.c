#include "stack_linked.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 128

/**
 * ### Compile:
 * % cmake -S . -B build
 * % make -C build
 *
 * ### Run:
 * % echo "I II III IV V - - x" > input.txt
 * % ./build/bin/stack_linked_client < input.txt
 *
 */
int main() {
  printf("** LinkedListStack **\n");
  printf("=====================\n");
  printf("\n");

  char inputBuffer[BUFFER_SIZE];
  struct LinkedStack _stack, *stack = &_stack;
  Stack_Init(stack);

  while (fscanf(stdin, "%127s", inputBuffer) != EOF) {
    if (strncmp(inputBuffer, "x\0", 2) == 0) {
      break;
    }

    if (strncmp(inputBuffer, "-\0", 2) == 0) {
      if (Stack_IsEmpty(stack)) {
        printf("Stack is empty\n");
        continue;
      }

      char *popped = (char *)Stack_Pop(stack);
      printf("pop() = %s\n", popped);
      free(popped), (popped = NULL);
      continue;
    }

    size_t bufLen = strlen(inputBuffer);
    char *input = calloc(bufLen + 1, sizeof(*input));
    snprintf(input, bufLen + 1, "%s", inputBuffer);
    printf("push(%s)\n", input);
    Stack_Push(stack, input);
  }

  printf("\n");
  printf("isEmpty() = %s\n", Stack_IsEmpty(stack) ? "true" : "false");
  printf("size() = %d\n", Stack_Size(stack));
  printf("peek() = %s\n", (char *)Stack_Peek(stack));

  struct LinkedStackIterator _iterator, *iterator = &_iterator;
  StackIterator_Init(iterator, stack);

  while (StackIterator_HasNext(iterator)) {
    char *input = (char *)StackIterator_GetNext(iterator);
    printf("iterator_next() = %s\n", input);
  }
  printf("\n");

  StackIterator_Clear(iterator), (iterator = NULL);

  while (!Stack_IsEmpty(stack)) {
    free((char *)Stack_Pop(stack));
  }
  Stack_Clear(stack), (stack = NULL);

  return 0;
}

#undef BUFFER_SIZE
