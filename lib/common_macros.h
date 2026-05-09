#include <stdbool.h>

#ifndef __common_macros_h
#define __common_macros_h

#define IS_NULL(ptr) (NULL == ptr)
#define IS_NOT_NULL(ptr) (NULL != ptr)
#define BOOL_TO_STRING(val) (val == true ? "true" : val == false ? "false" : "<invalid>")

#define SUCCESS 0
#define ERROR -1

#endif // __common_macros_h

