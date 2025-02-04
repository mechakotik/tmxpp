cmake_minimum_required(VERSION 3.30)
project(neotmx)

set(CMAKE_CXX_STANDARD 20)

add_library(neotmx STATIC
        src/map.cpp
        src/tileset.cpp
        src/properties.cpp
        external/tinyxml2/tinyxml2.cpp
)

target_include_directories(neotmx PRIVATE
        include
        external/tinyxml2
)
