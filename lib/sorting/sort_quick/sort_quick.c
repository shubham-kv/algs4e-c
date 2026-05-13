#include <assert.h>
#include <errno.h>
#include <memory.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "arr_utils.h"
#include "common_macros.h"
#include "sort_insertion.h"
#include "sort_quick.h"
#include "std_random.h"

#define CUT_OFF 5

static int partition(void *arr, size_t width, const int low, const int high,
                     ComparatorFn cmp) {
  const int p = low;
  int i = low, j = high + 1;
  const void *pItem = arr + p * width;

  while (true) {
    while (cmp(arr + (++i) * width, pItem) < 0 && (i < high)) {
    }

    while (cmp(pItem, arr + (--j) * width) < 0 && (j > low)) {
    }

    if (i >= j) {
      break;
    }

    ArrUtils_Swap(arr, width, i, j);
  }

  ArrUtils_Swap(arr, width, p, j);
  return j;
}

static void sort(void *arr, size_t width, int low, int high, ComparatorFn cmp) {
  if (!(low < high)) {
    return;
  }

  const int n = (high - low) + 1;
  int p = partition(arr, width, low, high, cmp);
  sort(arr, width, low, p - 1, cmp);
  sort(arr, width, p + 1, high, cmp);
}

int Sort_Quick(void *arr, size_t width, int n, ComparatorFn cmp) {
  REQUIRE_TRUE(IS_NOT_NULL(arr), EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(width > 0, EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(n > 0, EINVAL, EXIT_FAILURE);
  REQUIRE_TRUE(IS_NOT_NULL(cmp), EINVAL, EXIT_FAILURE);

  StdRandom_seed(time(NULL));
  StdRandom_shuffle(arr, width, n);
  sort(arr, width, 0, n - 1, cmp);

  assert(ArrUtils_IsSorted(arr, width, n, cmp) == true);
  return EXIT_SUCCESS;
}

#undef CUT_OFF
