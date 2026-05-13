#include <assert.h>
#include <errno.h>
#include <memory.h>
#include <stdbool.h>
#include <stdlib.h>

#include "arr_utils.h"
#include "common_macros.h"
#include "sort_heap.h"

void sinkDown(void *arr, size_t width, int n, int p, ComparatorFn cmp) {
  for (int c = 2 * p + 1; c < n; p = c, c = 2 * p + 1) {
    if (c + 1 < n && cmp(arr + c * width, arr + (c + 1) * width) < 0) {
      c += 1;
    }
    if (cmp(arr + p * width, arr + c * width) < 0) {
      ArrUtils_Swap(arr, width, p, c);
    } else {
      break;
    }
  }
}

int Sort_Heap(void *arr, size_t width, int n, ComparatorFn cmp) {
  REQUIRE_TRUE(IS_NOT_NULL(arr), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(width > 0, EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(n > 0, EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(cmp), EINVAL, EXIT_FAILURE);

  for (int p = (n - 1 - 1) / 2; p >= 0; p--) {
    sinkDown(arr, width, n, p, cmp);
  }

  for (int s = n; s > 0;) {
    ArrUtils_Swap(arr, width, 0, --s);
    sinkDown(arr, width, s, 0, cmp);
  }

  assert(ArrUtils_IsSorted(arr, width, n, cmp) == true);
  return EXIT_SUCCESS;
}
