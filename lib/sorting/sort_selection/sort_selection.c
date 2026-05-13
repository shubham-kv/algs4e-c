#include <assert.h>
#include <errno.h>
#include <memory.h>
#include <stdbool.h>
#include <stdlib.h>

#include "arr_utils.h"
#include "common_macros.h"
#include "sort_selection.h"

int Sort_Selection(void *arr, size_t width, size_t n, ComparatorFn cmp) {
  REQUIRE_TRUE(IS_NOT_NULL(arr), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(width > 0, EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(n > 0, EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(cmp), EINVAL, EXIT_FAILURE);

  for (size_t i = 0; i < n; i++) {
    size_t min = i;

    for (size_t j = i + 1; j < n; j++) {
      if (cmp(arr + width * j, arr + width * min) < 0) {
        min = j;
      }
    }
    ArrUtils_Swap(arr, width, i, min);
  }

  assert(ArrUtils_IsSorted(arr, width, n, cmp) == true);
  return EXIT_SUCCESS;
}
