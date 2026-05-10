#include "stack_int.h"
#include <criterion/criterion.h>

static struct IntegerStack _stack, *stack = &_stack;

static void setup() {
  const int code = IntStack_Init(stack);
  cr_assert_eq(code, EXIT_SUCCESS);
}

static void teardown() {
  const int code = IntStack_Clear(stack);
  cr_assert_eq(code, EXIT_SUCCESS);
}


Test(
    int_stack,
    initializes_to_valid_state,
    .init = setup,
    .fini = teardown
) {
  IntStackItem integer;
  cr_assert_eq(IntStack_IsEmpty(stack), true);
  cr_assert_eq(IntStack_Size(stack), 0);
  cr_assert_neq(IntStack_Peek(stack, &integer), EXIT_SUCCESS);
  cr_assert_neq(IntStack_Pop(stack, &integer), EXIT_SUCCESS);

  struct IntegerStackIterator _iterator, *iterator = &_iterator;
  IntStackIter_Init(iterator, stack);
  cr_assert_eq(IntStackIter_HasNext(iterator), false);
  cr_assert_eq(IntStackIter_GetNext(iterator, &integer), EXIT_FAILURE);
  IntStackIter_Clear(iterator), (iterator = NULL);
}


Test(
    int_stack,
    allows_pushing_arbitrary_number_of_items,
    .init = setup,
    .fini = teardown
) {
  const int n = 1000256;
  IntStackItem arr[n];
  for (int i = 0; i < n; i++) {
    arr[i] = i;
    cr_expect_eq(IntStack_Push(stack, arr[i]), EXIT_SUCCESS);
  }

  const int expected = n;
  const int actual = IntStack_Size(stack);
  cr_expect_eq(actual, expected, "Expected stack size to be %d, got %d",
               expected, actual);
}


Test(
    int_stack,
    updates_state_correctly_after_every_push,
    .init = setup,
    .fini = teardown
) {
  const int n = 10;
  const IntStackItem arr[n] = {11, 20, 33, 44, 55, 66, 77, 88, 99, 110};

  IntStackItem integer;

  for (int i = 0; i < n; i++) {
    cr_assert_eq(IntStack_Push(stack, arr[i]), EXIT_SUCCESS);
    cr_expect_eq(IntStack_IsEmpty(stack), false);
    cr_expect_eq(IntStack_Size(stack), i + 1);
    cr_expect_eq(IntStack_Peek(stack, &integer), EXIT_SUCCESS);
    cr_expect_eq(integer, arr[i]);
  }
}


Test(
    int_stack,
    pushes_items_in_correct_order,
    .init = setup,
    .fini = teardown
) {
  const int n = 10;
  const IntStackItem arr[n] = {11, 20, 33, 44, 55, 66, 77, 88, 99, 110};
  for (int i = 0; i < n; i++) {
    cr_assert_eq(IntStack_Push(stack, arr[i]), EXIT_SUCCESS);
  }

  int i;
  IntStackItem actual, expected;
  struct IntegerStackIterator _iterator, *iterator = &_iterator;

  cr_expect_eq(IntStackIter_Init(iterator, stack), EXIT_SUCCESS);
  cr_expect_eq(IntStackIter_HasNext(iterator), true);

  // Items should be in reverse order
  for (i = n - 1; IntStackIter_HasNext(iterator); i--) {
    cr_assert(i >= 0);
    expected = arr[i];
    cr_expect_eq(IntStackIter_GetNext(iterator, &actual), EXIT_SUCCESS);
    cr_expect_eq(actual, expected, "Expected next integer to be %lld, got %lld",
                 expected, actual);
  }
  cr_expect_eq(i, -1); // Assert that all the items are consumed

  cr_expect_eq(IntStackIter_Clear(iterator), EXIT_SUCCESS);
}


Test(
    int_stack,
    updates_state_correctly_after_every_pop,
    .init = setup,
    .fini = teardown
) {
  const int n = 10;
  const IntStackItem arr[n] = {11, 20, 33, 44, 55, 66, 77, 88, 99, 110};
  for (int i = 0; i < n; i++) {
    cr_assert_eq(IntStack_Push(stack, arr[i]), EXIT_SUCCESS);
  }

  IntStackItem integer;

  for (int i = 0; i < n; i++) {
    cr_assert_eq(IntStack_Pop(stack, &integer), EXIT_SUCCESS);

    const bool expectedEmpty = i == n - 1;
    const int expectedStackSize = n - 1 - i;

    cr_expect_eq(IntStack_IsEmpty(stack), expectedEmpty);
    cr_expect_eq(IntStack_Size(stack), expectedStackSize);

    if (expectedEmpty) {
      cr_expect_eq(IntStack_Peek(stack, &integer), EXIT_FAILURE);
    } else {
      const IntStackItem expectedPeekedItem = arr[n - 2 - i];
      cr_expect_eq(IntStack_Peek(stack, &integer), EXIT_SUCCESS);
      cr_expect_eq(integer, expectedPeekedItem);
    }
  }

  struct IntegerStackIterator _iterator, *iterator = &_iterator;
  cr_expect_eq(IntStackIter_Init(iterator, stack), EXIT_SUCCESS);
  cr_expect_eq(IntStackIter_HasNext(iterator), false);
  cr_expect_neq(IntStackIter_GetNext(iterator, &integer), EXIT_SUCCESS);
  cr_expect_eq(IntStackIter_Clear(iterator), EXIT_SUCCESS);
}


Test(
    int_stack,
    pops_items_in_correct_order,
    .init = setup,
    .fini = teardown
) {
  const int n = 10;
  const IntStackItem arr[n] = {11, 20, 33, 44, 55, 66, 77, 88, 99, 110};
  for (int i = 0; i < n; i++) {
    cr_assert_eq(IntStack_Push(stack, arr[i]), EXIT_SUCCESS);
  }

  IntStackItem actual, expected;

  for (int i = n - 1; i >= 0; i--) {
    expected = arr[i];
    cr_expect_eq(IntStack_Pop(stack, &actual), EXIT_SUCCESS);
    cr_expect_eq(actual, expected, "Expected popped integer to be %lld, got %lld",
              expected, actual);
  }
}

