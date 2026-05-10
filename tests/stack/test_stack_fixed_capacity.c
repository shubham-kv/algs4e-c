#include "stack_fixed_capacity.h"
#include <criterion/criterion.h>

#define CAPACITY 8

static struct FixedCapacityStack _stack, *stack = &_stack;

static void setup() {
  Stack_Init(stack, CAPACITY);
}

static void teardown() {
  Stack_Clear(stack), (stack = NULL);
}

Test(
    fixed_capacity_stack,
    initializes_to_valid_state,
    .init = setup,
    .fini = teardown
) {
  cr_assert_eq(Stack_Pop(stack), NULL);
  cr_assert_eq(Stack_Peek(stack), NULL);
  cr_assert_eq(Stack_Size(stack), 0);
  cr_assert_eq(Stack_IsEmpty(stack), true);

  struct FCStackIterator _iterator, *iterator = &_iterator;
  StackIterator_Init(iterator, stack);

  cr_assert_eq(StackIterator_HasNext(iterator), false);
  cr_assert_eq(StackIterator_GetNext(iterator), NULL);

  StackIterator_Clear(iterator), (iterator = NULL);
}

Test(
    fixed_capacity_stack,
    updates_state_correctly_after_every_push,
    .init = setup,
    .fini = teardown
) {
  const int arrSize = CAPACITY;
  const int arr[arrSize] = {11, 20, 33, 44, 55, 66, 77, 88};

  for (int i = 0; i < arrSize; i++) {
    Stack_Push(stack, &arr[i]);

    cr_assert_eq(Stack_IsEmpty(stack), false);
    cr_assert_eq(Stack_Size(stack), i + 1);
    cr_assert_eq(*((int *) Stack_Peek(stack)), arr[i]);
  }
}

Test(
    fixed_capacity_stack,
    pushes_items_in_correct_order,
    .init = setup,
    .fini = teardown
) {
  const int arrSize = CAPACITY;
  const int arr[arrSize] = {11, 20, 33, 44, 55, 66, 77, 88};
  for (int i = 0; i < arrSize; i++) {
    Stack_Push(stack, &arr[i]);
  }

  int actual, expected;
  struct FCStackIterator _iterator, *iterator = &_iterator;
  StackIterator_Init(iterator, stack);
  cr_assert_eq(StackIterator_HasNext(iterator), true);

  // Items should be iterated over in reverse order
  for (int i = arrSize - 1; StackIterator_HasNext(iterator); i--) {
    cr_assert(i >= 0);
    expected = arr[i];
    actual = *((int *)StackIterator_GetNext(iterator));
    cr_assert(actual == expected, "Expected next integer to be %d, got %d",
              expected, actual);
  }

  StackIterator_Clear(iterator), (iterator = NULL);
}

Test(
    fixed_capacity_stack,
    updates_state_correctly_after_every_pop,
    .init = setup,
    .fini = teardown
) {
  const int arrSize = CAPACITY;
  const int arr[arrSize] = {11, 20, 33, 44, 55, 66, 77, 88};
  for (int i = 0; i < arrSize; i++) {
    Stack_Push(stack, &arr[i]);
  }

  for (int i = 0; i < arrSize; i++) {
    Stack_Pop(stack);

    bool expectedEmpty = i == arrSize - 1;
    int expectedStackSize = arrSize - 1 - i;
    Item expectedPeekedItem = expectedEmpty ? NULL : &arr[arrSize - 2 - i];
    cr_assert_eq(Stack_IsEmpty(stack), expectedEmpty);
    cr_assert_eq(Stack_Size(stack), expectedStackSize);
    cr_assert_eq(Stack_Peek(stack), expectedPeekedItem);
  }

  struct FCStackIterator _iterator, *iterator = &_iterator;
  StackIterator_Init(iterator, stack);

  cr_assert_eq(StackIterator_HasNext(iterator), false);
  cr_assert_eq(StackIterator_GetNext(iterator), NULL);

  StackIterator_Clear(iterator), (iterator = NULL);
}

Test(
    fixed_capacity_stack,
    pops_items_in_correct_order,
    .init = setup,
    .fini = teardown
) {
  const int arrSize = CAPACITY;
  const int arr[arrSize] = {11, 20, 33, 44, 55, 66, 77, 88};
  for (int i = 0; i < arrSize; i++) {
    Stack_Push(stack, &arr[i]);
  }

  int actual, expected;

  // Items should be removed in reverse order
  for (int i = arrSize - 1; !Stack_IsEmpty(stack); i--) {
    cr_assert(i >= 0);
    expected = arr[i];
    actual = *(int *) Stack_Pop(stack);
    cr_assert(actual == expected, "Expected popped integer to be %d, got %d",
              expected, actual);
  }
}

