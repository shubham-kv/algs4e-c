add_library(bag_int SHARED bag_int.c)
target_include_directories(bag_int PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(bag_int PRIVATE linked_node)
