#include <criterion/criterion.h>

#include "arr_utils.h"
#include "comparators.h"
#include "st_sequential_search.h"
#include "test_utils.h"

static SSST st = NULL;

static void setup() {
  st = SSST_Create(CMP_Int);
}

static void teardown() {
  SSST_Delete(&st);
}


Test(
    sequential_search_st,
    initializes_to_valid_state,
    .init = setup,
    .fini = teardown
) {
  cr_assert_neq(st, NULL);

  cr_expect_eq(SSST_Size(st), 0);
  cr_expect_eq(SSST_IsEmpty(st), true);

  SSSTKeysIter keysIterator = NULL;
  keysIterator = SSSTKeysIter_Create(st);

  SSSTKey nextKey;
  cr_assert_neq(keysIterator, NULL);
  cr_expect_eq(SSSTKeysIter_HasNext(keysIterator), false);
  cr_expect_neq(SSSTKeysIter_GetNext(keysIterator, &nextKey), EXIT_SUCCESS);

  cr_assert_eq(SSSTKeysIter_Delete(&keysIterator), EXIT_SUCCESS);
}


Test(
    sequential_search_st,
    correctly_updates_state_after_put,
    .init = setup,
    .fini = teardown
) {
  cr_assert_neq(st, NULL);

  const int n = 3;
  const int keys[n] = {0, 1, 2};
  const int vals[n] = {10, 99, 78};

  for (int i = 0; i < n; i++) {
    SSSTKey key = &keys[i];
    SSSTVal val = &vals[i];
    cr_assert_eq(SSST_Put(st, key, val), EXIT_SUCCESS);

    cr_expect_eq(SSST_Size(st), i + 1);
    cr_expect_eq(SSST_IsEmpty(st), false);

    bool contains = false;
    cr_assert_eq(SSST_Contains(st, key, &contains), EXIT_SUCCESS);
    cr_expect_eq(contains, true);

    SSSTVal returnedVal;
    int expected = vals[i];
    cr_expect_eq(SSST_Get(st, key, &returnedVal), EXIT_SUCCESS);

    int actual = *(int *)returnedVal;
    cr_expect_eq(actual, expected, "Expected get() to return %d, got %d",
                 expected, actual);
  }
}


Test(
    sequential_search_st,
    allows_iteration_over_the_added_keys,
    .init = setup,
    .fini = teardown
) {
  cr_assert_neq(st, NULL);

  const int n = 10000;
  int keys[n], vals[n];

  for (int i = 0; i < n; i++) {
    keys[i] = i;
    vals[i] = i;
    cr_assert_eq(SSST_Put(st, &keys[i], &vals[i]), EXIT_SUCCESS);
  }

  SSSTKeysIter keysIterator = NULL;
  keysIterator = SSSTKeysIter_Create(st);
  cr_assert_neq(keysIterator, NULL);

  int i;
  SSSTKey stKey;
  int keyIndices[n]; // Indices of the returned key in the original array

  for (i = 0; SSSTKeysIter_HasNext(keysIterator); i++) {
    const int code = SSSTKeysIter_GetNext(keysIterator, &stKey);
    const int key = *((int *)stKey);
    const int keyIndex = Search_Binary(keys, n, key);
    keyIndices[i] = keyIndex;

    cr_assert_eq(code, EXIT_SUCCESS);

    // Expect key to exist in original keys array
    cr_expect_eq(0 <= keyIndex && keyIndex < n, true); 

    bool contains = false;
    cr_assert_eq(SSST_Contains(st, stKey, &contains), EXIT_SUCCESS);
    cr_expect_eq(contains, true);
  }

  const bool hasDuplicateKeys = ArrUtils_ContainsDuplicates(keyIndices,
      sizeof(keyIndices[0]), n, CMP_Int);

  // Returned keys must be unique
  cr_expect_eq(hasDuplicateKeys, false);
  cr_expect_eq(i, n);

  cr_assert_eq(SSSTKeysIter_Delete(&keysIterator), EXIT_SUCCESS);
}


Test(
    sequential_search_st,
    correctly_updates_state_after_delete,
    .init = setup,
    .fini = teardown
) {
  const int n = 10000;
  int keys[n], vals[n];

  for (int i = 0; i < n; i++) {
    keys[i] = i;
    vals[i] = i;
    cr_assert_eq(SSST_Put(st, &keys[i], &vals[i]), EXIT_SUCCESS);
  }

  for (int i = 0; i < n; i++) {
    SSSTKey key = &keys[i];
    cr_assert_eq(SSST_DeleteKey(st, key), EXIT_SUCCESS);

    bool contains = false;
    cr_assert_eq(SSST_Contains(st, key, &contains), EXIT_SUCCESS);
    cr_expect_eq(contains, false);

    SSSTVal val = NULL;
    cr_assert_eq(SSST_Get(st, key, &val), EXIT_SUCCESS);
    cr_expect_eq(val, NULL);

    cr_expect_eq(SSST_Size(st), n - i - 1);
  }
}
