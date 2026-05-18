add_library(stack_int SHARED stack_int.c)
target_include_directories(stack_int PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(stack_int PRIVATE linked_node)
