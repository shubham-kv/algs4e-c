#include "queue_int.h"
#include <criterion/criterion.h>

static IntQueue queue;

static void setup() {
  queue = IntQueue_Create();
  cr_assert(queue != NULL);
}

static void teardown() {
  cr_assert_eq(IntQueue_Delete(&queue), EXIT_SUCCESS);
}


Test(
    int_queue,
    initializes_to_valid_state,
    .init = setup,
    .fini = teardown
) {
  IntQueueItem integer;
  cr_expect_eq(IntQueue_IsEmpty(queue), true);
  cr_expect_eq(IntQueue_Size(queue), 0);
  cr_expect_neq(IntQueue_Peek(queue, &integer), EXIT_SUCCESS);

  struct IntegerQueueIterator _iterator, *iterator = &_iterator;
  IntQueueIter_Init(iterator, queue);
  cr_expect_eq(IntQueueIter_HasNext(iterator), false);
  cr_expect_neq(IntQueueIter_GetNext(iterator, &integer), EXIT_SUCCESS);
  IntQueueIter_Clear(iterator), (iterator = NULL);
}


Test(
    int_queue,
    allows_adding_arbitrary_number_of_items,
    .init = setup,
    .fini = teardown
) {
  const int n = 1000128;
  IntQueueItem arr[n];
  for (int i = 0; i < n; i++) {
    arr[i] = i;
    cr_expect_eq(IntQueue_Enqueue(queue, arr[i]), EXIT_SUCCESS);
  }

  cr_expect_eq(IntQueue_Size(queue), n);
}


Test(
    int_queue,
    keeps_state_valid_after_every_enqueue,
    .init = setup,
    .fini = teardown
) {
  const int n = 10;
  const int arr[n] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  IntQueueItem integer;

  for (int i = 0; i < n; i++) {
    cr_assert_eq(IntQueue_Enqueue(queue, arr[i]), EXIT_SUCCESS);

    cr_expect_eq(IntQueue_IsEmpty(queue), false);
    cr_expect_eq(IntQueue_Size(queue), i + 1);
    cr_expect_eq(IntQueue_Peek(queue, &integer), EXIT_SUCCESS);
    cr_expect_eq(integer, arr[0]);
  }
}


Test(
    int_queue,
    enqueues_items_in_correct_order,
    .init = setup,
    .fini = teardown
) {
  const int n = 10;
  const int arr[n] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  for (int i = 0; i < n; i++) {
    cr_assert_eq(IntQueue_Enqueue(queue, arr[i]), EXIT_SUCCESS);
  }

  IntQueueItem i, actual, expected;
  struct IntegerQueueIterator _iterator, *iterator = &_iterator;
  cr_assert_eq(IntQueueIter_Init(iterator, queue), EXIT_SUCCESS);

  // Expect items to be iterated over in FIFO order
  for (i = 0; IntQueueIter_HasNext(iterator); i++) {
    expected = arr[i];
    cr_expect_eq(IntQueueIter_GetNext(iterator, &actual), EXIT_SUCCESS);
    cr_expect_eq(actual, expected, "Expected next integer to be %lld, got %lld",
                 expected, actual);
  }
  cr_expect_eq(i, n); // Expect all items to be consumed

  cr_assert_eq(IntQueueIter_Clear(iterator), EXIT_SUCCESS);
}


Test(
    int_queue,
    keeps_state_valid_after_every_dequeue,
    .init = setup,
    .fini = teardown
) {
  const int n = 10;
  const int arr[n] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  for (int i = 0; i < n; i++) {
    cr_assert_eq(IntQueue_Enqueue(queue, arr[i]), EXIT_SUCCESS);
  }

  IntQueueItem integer;

  for (int i = 0; i < n; i++) {
    cr_assert_eq(IntQueue_Dequeue(queue, &integer), EXIT_SUCCESS);

    const bool expectedEmpty = i == n - 1;
    const int expectedQueueSize = n - 1 - i;
    cr_expect_eq(IntQueue_IsEmpty(queue), expectedEmpty);
    cr_expect_eq(IntQueue_Size(queue), expectedQueueSize);

    if (expectedEmpty) {
      cr_expect_neq(IntQueue_Peek(queue, &integer), EXIT_SUCCESS);
    } else {
      IntQueueItem expectedPeek, actualPeek;
      expectedPeek = arr[i + 1];
      cr_expect_eq(IntQueue_Peek(queue, &actualPeek), EXIT_SUCCESS);
      cr_expect_eq(actualPeek, expectedPeek);
    }
  }

  struct IntegerQueueIterator _iterator, *iterator = &_iterator;
  cr_assert_eq(IntQueueIter_Init(iterator, queue), EXIT_SUCCESS);
  cr_expect_eq(IntQueueIter_HasNext(iterator), false);
  cr_expect_neq(IntQueueIter_GetNext(iterator, &integer), EXIT_SUCCESS);
  cr_assert_eq(IntQueueIter_Clear(iterator), EXIT_SUCCESS);
}


Test(
    int_queue,
    dequeues_items_in_correct_order,
    .init = setup,
    .fini = teardown
) {
  const int n = 10;
  const int arr[n] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  for (int i = 0; i < n; i++) {
    cr_assert_eq(IntQueue_Enqueue(queue, arr[i]), EXIT_SUCCESS);
  }

  int i;
  IntQueueItem actual, expected;

  // Expect to deque in FIFO order
  for (i = 0; !IntQueue_IsEmpty(queue); i++) {
    expected = arr[i];
    cr_expect_eq(IntQueue_Dequeue(queue, &actual), EXIT_SUCCESS);
    cr_expect_eq(actual, expected,
                 "Expected dequeued integer to be %lld, got %lld", expected,
                 actual);
  }
  cr_expect_eq(i, n); // Expect all items to be dequeued
}
