#include <criterion/criterion.h>

#include "queue_linked.h"

static struct LinkedQueue _queue, *queue = &_queue;

static void setup() {
  Queue_Init(queue);
}

static void teardown() {
  Queue_Clear(queue), (queue = NULL);
}


Test(
    linked_queue,
    initializes_to_valid_state,
    .init = setup,
    .fini = teardown
) {
  Item item;
  cr_expect_eq(Queue_IsEmpty(queue), true);
  cr_expect_eq(Queue_Size(queue), 0);
  cr_expect_neq(Queue_Peek(queue, &item), EXIT_SUCCESS);

  struct LinkedQueueIterator _iterator, *iterator = &_iterator;
  cr_assert_eq(QueueIterator_Init(iterator, queue), EXIT_SUCCESS);

  cr_expect_eq(QueueIterator_HasNext(iterator), false);
  cr_expect_neq(QueueIterator_GetNext(iterator, &item), EXIT_SUCCESS);

  cr_assert_eq(QueueIterator_Clear(iterator), EXIT_SUCCESS);
}


Test(
    linked_queue,
    allows_adding_arbitrary_number_of_items,
    .init = setup,
    .fini = teardown
) {
  const int n = 1000123;
  int arr[n];
  for (int i = 0; i < n; i++) {
    arr[i] = i;
    cr_expect_eq(Queue_Enqueue(queue, &arr[i]), EXIT_SUCCESS);
  }

  cr_expect_eq(Queue_Size(queue), n);
}


Test(
    linked_queue,
    keeps_state_valid_after_every_enqueue,
    .init = setup,
    .fini = teardown
) {
  const int n = 10;
  const int arr[n] = {11, 20, 33, 44, 55, 66, 77, 88, 99, 100};

  Item item;

  for (int i = 0; i < n; i++) {
    cr_assert_eq(Queue_Enqueue(queue, &arr[i]), EXIT_SUCCESS);

    cr_expect_eq(Queue_IsEmpty(queue), false);
    cr_expect_eq(Queue_Size(queue), i + 1);
    cr_expect_eq(Queue_Peek(queue, &item), EXIT_SUCCESS);
    cr_expect_eq(*((int *)item), arr[0]);
  }
}


Test(
    linked_queue,
    enqueues_items_in_correct_order,
    .init = setup,
    .fini = teardown
) {
  const int n = 10;
  const int arr[n] = {11, 20, 33, 44, 55, 66, 77, 88, 99, 100};
  for (int i = 0; i < n; i++) {
    cr_assert_eq(Queue_Enqueue(queue, &arr[i]), EXIT_SUCCESS);
  }

  Item item;
  int i, actual, expected;
  struct LinkedQueueIterator _iterator, *iterator = &_iterator;
  cr_assert_eq(QueueIterator_Init(iterator, queue), EXIT_SUCCESS);

  // Expect items to be iterated over in FIFO order
  for (i = 0; QueueIterator_HasNext(iterator); i++) {
    expected = arr[i];
    cr_assert_eq(QueueIterator_GetNext(iterator, &item), EXIT_SUCCESS);
    actual = *((int *)item);
    cr_expect_eq(actual, expected, "Expected next integer to be %d, got %d",
                 expected, actual);
  }
  cr_expect_eq(i, n); // Expect all items to be consumed

  cr_assert_eq(QueueIterator_Clear(iterator), EXIT_SUCCESS);
}


Test(
    linked_queue,
    keeps_state_valid_after_every_dequeue,
    .init = setup,
    .fini = teardown
) {
  const int n = 10;
  const int arr[n] = {11, 20, 33, 44, 55, 66, 77, 88, 99, 100};
  for (int i = 0; i < n; i++) {
    cr_assert_eq(Queue_Enqueue(queue, &arr[i]), EXIT_SUCCESS);
  }

  Item item;

  for (int i = 0; i < n; i++) {
    cr_assert_eq(Queue_Dequeue(queue, &item), EXIT_SUCCESS);

    bool expectedEmpty = i == n - 1;
    int expectedQueueSize = n - 1 - i;
    cr_expect_eq(Queue_IsEmpty(queue), expectedEmpty);
    cr_expect_eq(Queue_Size(queue), expectedQueueSize);

    if (expectedEmpty) {
      cr_expect_neq(Queue_Peek(queue, &item), EXIT_SUCCESS);
    } else {
      Item expectedPeek, actualPeek;
      expectedPeek = &arr[i + 1];
      cr_expect_eq(Queue_Peek(queue, &actualPeek), EXIT_SUCCESS);
      cr_expect_eq(actualPeek, expectedPeek);
    }
  }

  struct LinkedQueueIterator _iterator, *iterator = &_iterator;
  cr_assert_eq(QueueIterator_Init(iterator, queue), EXIT_SUCCESS);

  cr_expect_eq(QueueIterator_HasNext(iterator), false);
  cr_expect_neq(QueueIterator_GetNext(iterator, &item), EXIT_SUCCESS);

  cr_assert_eq(QueueIterator_Clear(iterator), EXIT_SUCCESS);
}


Test(
    linked_queue,
    dequeues_items_in_correct_order,
    .init = setup,
    .fini = teardown
) {
  const int n = 10;
  const int arr[n] = {11, 20, 33, 44, 55, 66, 77, 88, 99, 100};
  for (int i = 0; i < n; i++) {
    cr_assert_eq(Queue_Enqueue(queue, &arr[i]), EXIT_SUCCESS);
  }

  Item item;
  int i, actual, expected;

  // Expect the items to be removed in FIFO order
  for (i = 0; !Queue_IsEmpty(queue); i++) {
    expected = arr[i];

    cr_assert_eq(Queue_Dequeue(queue, &item), EXIT_SUCCESS);
    actual = *(int *)item;

    cr_expect_eq(actual, expected, "Expected dequeued integer to be %d, got %d",
                 expected, actual);
  }
  cr_expect_eq(i, n); // Expect all items to be dequeued
}
