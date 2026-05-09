#include <criterion/criterion.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include "stack_int.h"

static struct IntegerStack _stack, *stack = &_stack;

static void setup() {
  IntStack_Init(stack);
  cr_assert(stack != NULL);
}

static void teardown() {
  IntStack_Clear(stack);
}

Test(
    int_stack_push,
    pushes_data_onto_stack,
    .init = setup,
    .fini = teardown
) {
  IntStack_Push(stack, -1);
  IntStack_Push(stack,  0);
  IntStack_Push(stack, 10);

  cr_assert(IntStack_Size(stack) == 3);
  cr_assert(IntStack_IsEmpty(stack) == false);

  struct IntegerStackIterator _iterator, *iterator = &_iterator;
  IntStackIter_Init(iterator, stack);

  IntStackItem integer;
  cr_assert(IntStackIter_HasNext(iterator) == true);
  cr_assert(IntStackIter_GetNext(iterator, &integer) == EXIT_SUCCESS);
  cr_assert(10 == integer);

  cr_assert(IntStackIter_HasNext(iterator) == true);
  cr_assert(IntStackIter_GetNext(iterator, &integer) == EXIT_SUCCESS);
  cr_assert(0 == integer);

  cr_assert(IntStackIter_HasNext(iterator) == true);
  cr_assert(IntStackIter_GetNext(iterator, &integer) == EXIT_SUCCESS);
  cr_assert(-1 == integer);

  cr_assert(IntStackIter_HasNext(iterator) == false);
  cr_assert(IntStackIter_GetNext(iterator, &integer) == EXIT_FAILURE);

  IntStackIter_Clear(iterator);
}


Test(
    int_stack_pop,
    pops_data_from_stack,
    .init = setup,
    .fini = teardown
) {
  IntStack_Push(stack, -1);
  IntStack_Push(stack,  0);
  IntStack_Push(stack, 10);

  IntStackItem integer;
  cr_assert(IntStack_IsEmpty(stack) == false);
  cr_assert(IntStack_Pop(stack, &integer) == EXIT_SUCCESS);
  cr_assert(integer == 10);

  cr_assert(IntStack_IsEmpty(stack) == false);
  cr_assert(IntStack_Pop(stack, &integer) == EXIT_SUCCESS);
  cr_assert(integer == 0);

  cr_assert(IntStack_IsEmpty(stack) == false);
  cr_assert(IntStack_Pop(stack, &integer) == EXIT_SUCCESS);
  cr_assert(integer == -1);

  cr_assert(IntStack_IsEmpty(stack) == true);
  cr_assert(IntStack_Pop(stack, &integer) == EXIT_FAILURE);
}


Test(
    int_stack_push,
    exhaustive_pushes_onto_stack,
    .init = setup,
    .fini = teardown
) {
  const int integersSize = INT16_MAX;
  IntStackItem integers[integersSize];

  for (int i = 0; i < integersSize; i++) {
    integers[i] = i;
    cr_assert(IntStack_Push(stack, i) == EXIT_SUCCESS);
  }
  cr_assert(IntStack_Size(stack) == integersSize);

  int i = 0;
  struct IntegerStackIterator _iterator, *iterator = &_iterator;
  IntStackIter_Init(iterator, stack);

  cr_assert(IntStackIter_HasNext(iterator) == true);

  for (i = 0; IntStackIter_HasNext(iterator); i++) {
    IntStackItem actual = 0;
    cr_assert(IntStackIter_GetNext(iterator, &actual) == EXIT_SUCCESS);

    const IntStackItem expected = integers[(integersSize - 1) - i];
    cr_assert(actual == expected,
              "Expected next integer to be %lld, it is %lld", expected,
              actual);
  }

  cr_assert(i == integersSize, "Expected the iterator to consume all %d items, "
                               "but %d were consumed", integersSize, i);

  IntStackIter_Clear(iterator);
}


Test(
    int_stack_pop,
    exhaustive_pops_from_stack,
    .init = setup,
    .fini = teardown
) {
  const int integersSize = INT16_MAX;
  IntStackItem integers[integersSize];

  for (int i = 0; i < integersSize; i++) {
    integers[i] = i;
    cr_assert(IntStack_Push(stack, i) == EXIT_SUCCESS);
  }

  cr_assert(IntStack_Size(stack) == integersSize);

  IntStackItem i, actual;

  for (i = 0; !IntStack_IsEmpty(stack); i++) {
    const IntStackItem expected = integers[(integersSize - 1) - i];
    cr_assert(IntStack_Pop(stack, &actual) == EXIT_SUCCESS);
    cr_assert(actual == expected);
  }

  cr_assert(i == integersSize, "Expected the stack to pop all %d items, "
                               "but only %lld were popped", integersSize, i);
}

