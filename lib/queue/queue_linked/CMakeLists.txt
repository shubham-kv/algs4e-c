add_library(queue_linked SHARED queue_linked.c)
target_include_directories(queue_linked PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(queue_linked PRIVATE linked_node)
