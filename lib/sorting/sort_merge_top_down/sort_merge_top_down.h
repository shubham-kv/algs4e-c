#include <stddef.h>

#include "comparators.h"

#ifndef __top_down_merge_sort_h
#define __top_down_merge_sort_h

int Sort_MergeTopDown(void *arr, size_t width, int n, ComparatorFn cmp);

#endif // __top_down_merge_sort_h
