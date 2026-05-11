#include "queue_linked.h"
#include <criterion/criterion.h>

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
  cr_expect_eq(Queue_IsEmpty(queue), true);
  cr_expect_eq(Queue_Size(queue), 0);
  cr_expect_eq(Queue_Peek(queue), NULL);

  struct LinkedQueueIterator _iterator, *iterator = &_iterator;
  QueueIterator_Init(iterator, queue);
  cr_expect_eq(QueueIterator_HasNext(iterator), false);
  cr_expect_eq(QueueIterator_GetNext(iterator), NULL);
  QueueIterator_Clear(iterator), (iterator = NULL);
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
    Queue_Enqueue(queue, &arr[i]);
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

  for (int i = 0; i < n; i++) {
    Queue_Enqueue(queue, &arr[i]);
    cr_expect_eq(Queue_IsEmpty(queue), false);
    cr_expect_eq(Queue_Size(queue), i + 1);
    cr_expect_eq(*((int *)Queue_Peek(queue)), arr[0]);
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
    Queue_Enqueue(queue, &arr[i]);
  }

  int i, actual, expected;
  struct LinkedQueueIterator _iterator, *iterator = &_iterator;
  QueueIterator_Init(iterator, queue);

  // Expect items to be iterated over in FIFO order
  for (i = 0; QueueIterator_HasNext(iterator); i++) {
    expected = arr[i];
    actual = *((int *)QueueIterator_GetNext(iterator));
    cr_expect_eq(actual, expected, "Expected next integer to be %d, got %d",
                 expected, actual);
  }
  cr_expect_eq(i, n); // Expect all items to be consumed

  QueueIterator_Clear(iterator), (iterator = NULL);
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
    Queue_Enqueue(queue, &arr[i]);
  }

  for (int i = 0; i < n; i++) {
    Queue_Dequeue(queue);

    bool expectedEmpty = i == n - 1;
    int expectedQueueSize = n - 1 - i;
    Item expectedPeek = expectedEmpty ? NULL : &arr[i + 1];
    cr_expect_eq(Queue_IsEmpty(queue), expectedEmpty);
    cr_expect_eq(Queue_Size(queue), expectedQueueSize);
    cr_expect_eq(Queue_Peek(queue), expectedPeek);
  }

  struct LinkedQueueIterator _iterator, *iterator = &_iterator;
  QueueIterator_Init(iterator, queue);
  cr_expect_eq(QueueIterator_HasNext(iterator), false);
  cr_expect_eq(QueueIterator_GetNext(iterator), NULL);
  QueueIterator_Clear(iterator), (iterator = NULL);
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
    Queue_Enqueue(queue, &arr[i]);
  }

  int i, actual, expected;

  // Expect the items to be removed in FIFO order
  for (i = 0; !Queue_IsEmpty(queue); i++) {
    expected = arr[i];
    actual = *(int *)Queue_Dequeue(queue);
    cr_expect_eq(actual, expected, "Expected dequeued integer to be %d, got %d",
                 expected, actual);
  }
  cr_expect_eq(i, n); // Expect all items to be dequeued
}
