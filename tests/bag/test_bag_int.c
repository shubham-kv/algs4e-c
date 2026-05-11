#include "bag_int.h"
#include <criterion/criterion.h>

// Utilities
int binarySearch(const int arr[], const int n, const int target);
bool containsDuplicates(const int arr[], const int n);

static IntBag bag;

static void setup() {
  bag = IntBag_Create();
  cr_assert_neq(bag, NULL);
}

static void teardown() {
  cr_assert_eq(IntBag_Delete(&bag), EXIT_SUCCESS);
}


Test(
    int_bag,
    initializes_to_valid_state,
    .init = setup,
    .fini = teardown
) {
  IntBagItem item;
  cr_expect_eq(IntBag_IsEmpty(bag), true);
  cr_expect_eq(IntBag_Size(bag), 0);

  IntBagIterator iterator = IntBagIter_Create(bag);
  cr_assert_neq(iterator, NULL);
  cr_expect_eq(IntBagIter_HasNext(iterator), false);
  cr_expect_neq(IntBagIter_GetNext(iterator, &item), EXIT_SUCCESS);
  cr_assert_eq(IntBagIter_Delete(&iterator), EXIT_SUCCESS);
}


Test(
    int_bag,
    allows_to_add_arbitrary_number_of_items,
    .init = setup,
    .fini = teardown
) {
  const int n = 1000256;
  int integers[n];
  for (int i = 0; i < n; i++) {
    integers[i] = i;
    cr_expect_eq(IntBag_Add(bag, integers[i]), EXIT_SUCCESS);
  }

  cr_expect_eq(IntBag_Size(bag), n);
}


Test(
    int_bag,
    keeps_state_valid_after_every_addition,
    .init = setup,
    .fini = teardown
) {
  const int n = 10;
  const int integers[n] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  for (int i = 0; i < n; i++) {
    cr_assert_eq(IntBag_Add(bag, integers[i]), EXIT_SUCCESS);
    cr_expect_eq(IntBag_IsEmpty(bag), false);
    cr_expect_eq(IntBag_Size(bag), i + 1);
  }
}


Test(
    int_bag,
    bag_iterator_yields_all_added_integers,
    .init = setup,
    .fini = teardown
) {
  const int n = 10;
  const int integers[n] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  for (int i = 0; i < n; i++) {
    cr_assert_eq(IntBag_Add(bag, integers[i]), EXIT_SUCCESS);
  }

  IntBagIterator iterator = IntBagIter_Create(bag);
  cr_assert_neq(iterator, NULL);

  IntBagItem bagItem;
  int i, bagItemAsInt, bagItemIndex;
  int bagItemIndices[n];

  for (i = 0; IntBagIter_HasNext(iterator); i++) {
    cr_expect_eq(IntBagIter_GetNext(iterator, &bagItem), EXIT_SUCCESS);

    bagItemAsInt = (int) bagItem;
    bagItemIndex = binarySearch(integers, n, bagItemAsInt);
    bagItemIndices[i] = bagItemIndex; // Save the index to indices array

    // Expect item to exist in our original array
    cr_expect_eq(0 <= bagItemIndex && bagItemIndex < n, true);
  }

  // Expect iterator to not yield any duplicates
  cr_expect_eq(containsDuplicates(bagItemIndices, n), false);
  cr_expect_eq(i, n); // Expect all items to be consumed

  cr_assert_eq(IntBagIter_Delete(&iterator), EXIT_SUCCESS);
}


// Quick Utility deifinitions
// ==========================

int binarySearch(const int arr[], const int n, const int target) {
  int low = 0, high = n - 1;

  while (low <= high) {
    const int mid = (low + high) / 2;
    if (target < arr[mid]) {
      high = mid - 1;
    } else if (target > arr[mid]) {
      low = mid + 1;
    } else {
      return mid;
    }
  }
  return -1;
}

bool containsDuplicates(const int arr[], const int n) {
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      if (arr[i] == arr[j]) {
        return true;
      }
    }
  }
  return false;
}
