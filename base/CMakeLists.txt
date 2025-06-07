set(CMAKE_CXX_STANDARD 20)

file(GLOB BASIC_SRCS "*.cc")
list(FILTER BASIC_SRCS EXCLUDE REGEX ".*_test.cc")

add_library(base STATIC ${BASIC_SRCS})

find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(file_util_test "file_util_test.cc")
target_link_libraries(file_util_test base gtest gtest_main pthread)

add_executable(data_pool_test "data_pool_test.cc")
target_link_libraries(data_pool_test base gtest gtest_main pthread)

add_executable(node_pool_test "node_pool_test.cc")
target_link_libraries(node_pool_test base gtest gtest_main pthread)

add_executable(skiplist_test "skiplist_test.cc")
target_link_libraries(skiplist_test base gtest gtest_main pthread)

add_executable(thread_pool_test "thread_pool_test.cc")
target_link_libraries(thread_pool_test base gtest gtest_main pthread)
