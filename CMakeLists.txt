cmake_minimum_required(VERSION 3.30)
project(tmxpp)

option(TMXPP_TEST "Build tests" OFF)

set(CMAKE_CXX_STANDARD 20)

add_library(tmxpp STATIC
        src/map.cpp
        src/tileset.cpp
        src/properties.cpp
        src/utility.cpp
        src/abstract_layer.cpp
        src/tile_layer.cpp
        src/layer.cpp
        src/image.cpp
        src/tile.cpp
        src/object_group.cpp
        src/object.cpp
        external/tinyxml2/tinyxml2.cpp
)

target_include_directories(tmxpp PRIVATE
        include
        external/tinyxml2
)

if(TMXPP_TEST)
    add_subdirectory(external/googletest)
    enable_testing()

    add_executable(tmxpp_test
            test/basic.cpp
            test/external_tileset.cpp
    )

    target_link_libraries(tmxpp_test GTest::gtest_main tmxpp)
    target_include_directories(tmxpp_test PRIVATE include)
    file(COPY ${CMAKE_SOURCE_DIR}/test/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

    include(GoogleTest)
    gtest_discover_tests(tmxpp_test)
endif()
