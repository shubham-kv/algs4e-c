file(GLOB CHILD_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)

foreach(dir ${CHILD_DIRS})
  if (IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${dir})
    add_subdirectory(${dir})
  endif()
endforeach()

add_library(arr_utils SHARED arr_utils.c)
add_library(comparators SHARED comparators.c)
add_library(hashers SHARED hashers.c)
add_library(std_random SHARED std_random.c)

