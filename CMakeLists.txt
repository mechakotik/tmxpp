cmake_minimum_required(VERSION 3.30)
project(neotmx)

set(CMAKE_CXX_STANDARD 20)

add_library(neotmx STATIC
        src/map.cpp
        src/tileset.cpp
        src/properties.cpp
        src/utility.cpp
        src/layer.cpp
        external/tinyxml2/tinyxml2.cpp
)

target_include_directories(neotmx PRIVATE
        include
        external/tinyxml2
)

add_subdirectory(external/googletest)
enable_testing()

add_executable(neotmx_test test/basic.cpp)
target_link_libraries(neotmx_test GTest::gtest_main neotmx)
target_include_directories(neotmx_test PRIVATE include)
file(COPY ${CMAKE_SOURCE_DIR}/test/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

include(GoogleTest)
gtest_discover_tests(neotmx_test)
