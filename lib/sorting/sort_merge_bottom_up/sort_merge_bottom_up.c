#include <assert.h>
#include <errno.h>
#include <memory.h>
#include <stdbool.h>
#include <stdlib.h>

#include "arr_utils.h"
#include "common_macros.h"
#include "sort_merge_bottom_up.h"
#include "sort_merge_common.h"

#define MIN(a, b) (a < b ? a : b)

int Sort_MergeBottomUp(void *arr, size_t width, int n, ComparatorFn cmp) {
  REQUIRE_TRUE(IS_NOT_NULL(arr), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(width > 0, EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(n > 0, EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(cmp), EINVAL, EXIT_FAILURE);

  void *aux = calloc(n, width);

  for (int l = 1; l < n; l *= 2) {
    for (int low = 0; low + l < n; low += 2 * l) {
      const int mid = low + l - 1;
      const int high = MIN(low + 2 * l - 1, n - 1);

      if (cmp(arr + (mid + 1) * width, arr + mid * width) < 0) {
        merge(arr, aux, width, low, mid, high, cmp);
      }
    }
  }

  free(aux), (aux = NULL);
  assert(ArrUtils_IsSorted(arr, width, n, cmp) == true);
  return EXIT_SUCCESS;
}

#undef MIN
