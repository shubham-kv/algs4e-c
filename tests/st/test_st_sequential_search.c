#include <criterion/criterion.h>

#include "st_sequential_search.h"

static SSST st = NULL;

static void setup() {
  st = SSST_Create();
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
  const char *keys[n] = {"foo", "bar", "foobar"};
  const int vals[n] = {1, 2, 3};

  for (int i = 0; i < n; i++) {
    SSSTKey key = &keys[i];
    SSSTVal val = &vals[i];
    cr_assert_eq(SSST_Put(st, key, val), EXIT_SUCCESS);

    cr_expect_eq(SSST_Size(st), i + 1);
    cr_expect_eq(SSST_IsEmpty(st), false);
    cr_expect_eq(SSST_Contains(st, key), true);

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
  for (i = 0; SSSTKeysIter_HasNext(keysIterator); i++) {
    cr_assert_eq(SSSTKeysIter_GetNext(keysIterator, &stKey), EXIT_SUCCESS);
    cr_expect_eq(SSST_Contains(st, stKey), true);
  }
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

    SSSTVal val;
    cr_expect_eq(SSST_Contains(st, key), false);
    cr_expect_neq(SSST_Get(st, key, &val), EXIT_SUCCESS);
    cr_expect_eq(SSST_Size(st), n - i - 1);
  }
}
