#include "test_utils.h"

int Search_Binary(const int arr[], const int n, const int target) {
  int low = 0, high = n - 1;

  while (low <= high) {
    const int mid = (low + high) / 2;
    if (target < arr[mid]) {
      high = mid - 1;
    } else if (target > arr[mid]) {
      low = mid + 1;
    } else {
      return mid;
    }
  }
  return -1;
}
