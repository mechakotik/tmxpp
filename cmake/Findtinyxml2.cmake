find_path(TINYXML2_INCLUDE_DIRS
        NAMES tinyxml2.h
        HINTS ${tinyxml2_ROOT_DIR}/include)

find_library(TINYXML2_LIBRARIES
        NAMES tinyxml2
        HINTS ${tinyxml2_ROOT_DIR}/lib)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(tinyxml2 DEFAULT_MSG TINYXML2_LIBRARIES TINYXML2_INCLUDE_DIRS)

mark_as_advanced(
        TINYXML2_LIBRARIES
        TINYXML2_INCLUDE_DIRS)

if(TINYXML2_FOUND AND NOT (TARGET tinyxml2::tinyxml2))
    add_library(tinyxml2::tinyxml2 UNKNOWN IMPORTED)
    set_target_properties(tinyxml2::tinyxml2
            PROPERTIES
            IMPORTED_LOCATION ${TINYXML2_LIBRARIES}
            INTERFACE_INCLUDE_DIRECTORIES ${TINYXML2_INCLUDE_DIRS})
endif()
