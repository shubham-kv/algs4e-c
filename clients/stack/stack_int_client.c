#include "stack_int.h"
#include "common_macros.h"
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
 * % echo "10 20 30 40 - - 50 60 - x" > input.txt
 * % ./build/bin/stack_int_client < input.txt
 *
 */
int main() {
  printf("** IntStack **\n");
  printf("==============\n");
  printf("\n");

  char buffer[BUFFER_SIZE];
  struct IntegerStack _stack, *stack = &_stack;
  ENSURE_CALL_SUCCESS(IntStack_Init(stack));

  while (fscanf(stdin, "%63s", buffer) != EOF) {
    if (strncmp(buffer, "x\0", 2) == 0) {
      break;
    }
    else if (strncmp(buffer, "-\0", 2) == 0) {
      if (IntStack_IsEmpty(stack)) {
        printf("Stack is empty\n");
      } else {
        IntStackItem integer;
        ENSURE_CALL_SUCCESS(IntStack_Pop(stack, &integer));
        printf("pop() = %lld\n", integer);
      }
    }
    else {
      char *end;
      IntStackItem integer = (IntStackItem) strtol(buffer, &end, 10);
      if (end == buffer) {
        fprintf(stderr, "Invalid input!\n");
        return EXIT_FAILURE;
      }

      ENSURE_CALL_SUCCESS(IntStack_Push(stack, integer));
      printf("push(%lld)\n", integer);
    }

    memset(&buffer, 0, BUFFER_SIZE);
  }

  int code;
  IntStackItem integer;
  bool isStackEmpty = IntStack_IsEmpty(stack);

  printf("\n");
  printf("isEmpty() = %s\n", isStackEmpty ? "true" : "false");
  printf("size() = %d\n", IntStack_Size(stack));

  if (!isStackEmpty) {
    ENSURE_CALL_SUCCESS(IntStack_Peek(stack, &integer));
    printf("peek() = %lld\n", integer);
  }

  struct IntegerStackIterator _iterator, *iterator = &_iterator;
  ENSURE_CALL_SUCCESS(IntStackIter_Init(iterator, stack));

  while (IntStackIter_HasNext(iterator)) {
    ENSURE_CALL_SUCCESS(IntStackIter_GetNext(iterator, &integer));
    printf("iterator_next() = %lld\n", integer);
  }
  printf("\n");

  ENSURE_CALL_SUCCESS(IntStackIter_Clear(iterator));
  ENSURE_CALL_SUCCESS(IntStack_Clear(stack));
  stack = NULL;

  return EXIT_SUCCESS;
}
