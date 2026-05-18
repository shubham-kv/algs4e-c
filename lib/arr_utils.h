#include <stdbool.h>
#include <stddef.h>

#include "comparators.h"

#ifndef __arr_utils_h
#define __arr_utils_h

void ArrUtils_Swap(void *arr, size_t width, size_t i, size_t j);
bool ArrUtils_IsSorted(void *arr, size_t width, size_t n, ComparatorFn cmp);
bool ArrUtils_ContainsDuplicates(void *arr, size_t width, size_t n,
                                 ComparatorFn cmp);

#endif // __arr_utils_h
