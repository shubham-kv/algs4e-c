#include <assert.h>
#include <errno.h>
#include <memory.h>
#include <stdbool.h>
#include <stdlib.h>

#include "arr_utils.h"
#include "common_macros.h"
#include "sort_insertion.h"

int Sort_Insertion(void *arr, size_t width, int n, ComparatorFn cmp) {
  REQUIRE_TRUE(IS_NOT_NULL(arr), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(width > 0, EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(n > 0, EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(cmp), EINVAL, EXIT_FAILURE);

  for (int i = 1; i < n; i++) {
    for (int j = i; j - 1 >= 0; j--) {
      if (cmp(arr + j * width, arr + (j - 1) * width) < 0) {
        ArrUtils_Swap(arr, width, j, j - 1);
      } else {
        break;
      }
    }
  }

  assert(ArrUtils_IsSorted(arr, width, n, cmp) == true);
  return EXIT_SUCCESS;
}
