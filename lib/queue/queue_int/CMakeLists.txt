add_library(queue_int SHARED queue_int.c)
target_include_directories(queue_int PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(queue_int PRIVATE linked_node)
