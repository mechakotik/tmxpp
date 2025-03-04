cmake_minimum_required(VERSION 3.30)
project(tmxpp)

option(TMXPP_VENDORED "Use vendored third-party libraries" ON)
option(TMXPP_BASE64 "Support base64 encoded tilemaps" ON)
option(TMXPP_ZSTD "Support zstd compressed tilemaps" ON)
option(TMXPP_ZLIB "Support zlib and gzip compressed tilemaps" ON)
option(TMXPP_TEST "Build tests" OFF)

set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_library(tmxpp
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
)

target_include_directories(tmxpp PRIVATE
        include
        external/base64/include
)

if(TMXPP_VENDORED)
    set(tinyxml2_BUILD_TESTING OFF)
    add_subdirectory(external/tinyxml2 EXCLUDE_FROM_ALL)
    target_include_directories(tmxpp PRIVATE external/tinyxml2)
    target_link_libraries(tmxpp tinyxml2::tinyxml2)
else()
    find_package(tinyxml2 REQUIRED)
    target_include_directories(tmxpp PRIVATE ${TINYXML2_INCLUDE_DIRS})
    target_link_libraries(tmxpp tinyxml2::tinyxml2)
endif()

if(TMXPP_BASE64)
    target_compile_definitions(tmxpp PRIVATE TMXPP_BASE64)
endif()

if(TMXPP_ZSTD)
    target_compile_definitions(tmxpp PRIVATE TMXPP_ZSTD)
    if(TMXPP_VENDORED)
        set(ZSTD_BUILD_TESTS OFF)
        add_subdirectory(external/zstd/build/cmake EXCLUDE_FROM_ALL)
        target_include_directories(tmxpp PRIVATE external/zstd/lib)
        target_link_libraries(tmxpp libzstd_static)
    else()
        find_package(zstd REQUIRED)
        target_include_directories(tmxpp PRIVATE ${ZSTD_INCLUDE_DIRS})
        target_link_libraries(tmxpp zstd::zstd)
    endif()
endif()

if(TMXPP_ZLIB)
    target_compile_definitions(tmxpp PRIVATE TMXPP_ZLIB)
    if(TMXPP_VENDORED)
        set(ZLIB_BUILD_TESTING OFF)
        add_subdirectory(external/zlib EXCLUDE_FROM_ALL)
        target_include_directories(tmxpp PRIVATE external/zlib)
        target_link_libraries(tmxpp zlibstatic)
    else()
        find_package(ZLIB REQUIRED)
        target_include_directories(tmxpp PRIVATE ${ZLIB_INCLUDE_DIRS})
        target_link_libraries(tmxpp ZLIB::ZLIB)
    endif()
endif()

if(TMXPP_TEST)
    add_subdirectory(external/googletest)
    enable_testing()

    add_executable(tmxpp-test
            test/base64.cpp
            test/basic.cpp
            test/external_tileset.cpp
    )

    if(TMXPP_BASE64)
        target_compile_definitions(tmxpp-test PRIVATE TMXPP_BASE64)
    endif()
    if(TMXPP_ZSTD)
        target_compile_definitions(tmxpp-test PRIVATE TMXPP_ZSTD)
    endif()
    if(TMXPP_ZLIB)
        target_compile_definitions(tmxpp-test PRIVATE TMXPP_ZLIB)
    endif()

    target_link_libraries(tmxpp-test GTest::gtest_main tmxpp)
    target_include_directories(tmxpp-test PRIVATE include)
    file(COPY ${CMAKE_SOURCE_DIR}/test/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

    include(GoogleTest)
    gtest_discover_tests(tmxpp-test)
endif()
