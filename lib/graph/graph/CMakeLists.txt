add_library(graph SHARED graph.c)
target_include_directories(graph PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(graph PRIVATE bag_int)

