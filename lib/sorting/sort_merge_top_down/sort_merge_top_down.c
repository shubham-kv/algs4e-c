#include <assert.h>
#include <errno.h>
#include <memory.h>
#include <stdbool.h>
#include <stdlib.h>

#include "arr_utils.h"
#include "common_macros.h"
#include "sort_merge_common.h"
#include "sort_merge_top_down.h"

#define CUT_OFF 6

static void sort(void *arr, void *aux, size_t width, int low, int high,
                 ComparatorFn cmp) {
  if (!(low < high)) {
    return;
  }

  const int n = (high - low) + 1;
  const int mid = (low + high) / 2;

  sort(arr, aux, width, low, mid, cmp);
  sort(arr, aux, width, mid + 1, high, cmp);
  merge(arr, aux, width, low, mid, high, cmp);
}

int Sort_MergeTopDown(void *arr, size_t width, int n, ComparatorFn cmp) {
  REQUIRE_TRUE(IS_NOT_NULL(arr), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(width > 0, EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(n > 0, EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(cmp), EINVAL, EXIT_FAILURE);

  void *aux = calloc(n, width);
  sort(arr, aux, width, 0, n - 1, cmp);
  free(aux), (aux = NULL);

  assert(ArrUtils_IsSorted(arr, width, n, cmp) == true);
  return EXIT_SUCCESS;
}

#undef CUT_OFF
