#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifndef __common_macros_h
#define __common_macros_h

#define IS_NULL(ptr) (NULL == ptr)
#define IS_NOT_NULL(ptr) (NULL != ptr)
#define BOOL_TO_STRING(val)                                                    \
  (val == true ? "true" : val == false ? "false" : "<invalid>")

#define ENSURE_CALL_SUCCESS(_callExpr)                                         \
  do {                                                                         \
    int code = _callExpr;                                                      \
    if (code != EXIT_SUCCESS) {                                                \
      fprintf(stderr, "Error: %s\n", strerror(errno));                         \
      fprintf(stderr, "Call %s at %s:%d\n", #_callExpr, __FILE__, __LINE__);   \
      fprintf(stderr, "\n");                                                   \
      return code;                                                             \
    }                                                                          \
  } while (0)

#define ENSURE_SUCCESS(_callExpr)                                              \
  do {                                                                         \
    const int code = _callExpr;                                                \
    if (code != EXIT_SUCCESS) {                                                \
      return code;                                                             \
    }                                                                          \
  } while (0)

#define REQUIRE_TRUE(_expr, _errorCode, _returnVal)                            \
  do {                                                                         \
    bool isTrue = _expr;                                                       \
    if (!isTrue) {                                                             \
      errno = _errorCode;                                                      \
      return _returnVal;                                                       \
    }                                                                          \
  } while (0)

#define RETURN_IF_NULL(_expr, _returnVal)                                      \
  do {                                                                         \
    if ((_expr) == NULL) {                                                     \
      return _returnVal;                                                       \
    }                                                                          \
  } while (0)

#endif // __common_macros_h
