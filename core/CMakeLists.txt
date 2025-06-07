set(CMAKE_CXX_STANDARD 20)

file(GLOB CORE_SRCS "*.cc")

add_library(core STATIC ${CORE_SRCS})

find_package(GTest REQUIRED)
include(GoogleTest)