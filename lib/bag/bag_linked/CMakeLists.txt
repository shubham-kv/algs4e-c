add_library(bag_linked SHARED bag_linked.c)
target_include_directories(bag_linked PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(bag_linked PRIVATE linked_node)
