#include <stddef.h>

#include "comparators.h"

#ifndef __shell_sort_h
#define __shell_sort_h

int Sort_Shell(void *arr, size_t width, int n, ComparatorFn cmp);

#endif // __shell_sort_h
