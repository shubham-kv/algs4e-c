#include <stdbool.h>

#ifndef __common_macros_h
#define __common_macros_h

#define IS_NULL(ptr) (NULL == ptr)
#define IS_NOT_NULL(ptr) (NULL != ptr)
#define BOOL_TO_STRING(val) (val == true ? "true" : val == false ? "false" : "<invalid>")

#define ENSURE_CALL_SUCCESS(_callExpr) \
  do {                     \
    int code = _callExpr;  \
    if (code != 0) {       \
      fprintf(stderr, "Call %s failed at %s:%d\n\n", #_callExpr, __FILE__, __LINE__); \
      return code;         \
    }                      \
  } while (0)              \

#define REQUIRE_TRUE(_expr, _errorCode, _returnVal) \
  do {                     \
    bool isTrue = _expr;   \
    if (!isTrue) {         \
      errno = _errorCode;  \
      return _returnVal;  \
    }                      \
  } while (0)              \

#endif // __common_macros_h

