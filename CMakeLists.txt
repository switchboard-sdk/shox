cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

project(shox VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(GNUInstallDirs)

set(LIBRARY_NAME shox)
set(TARGET_NAME ${PROJECT_NAME}_${LIBRARY_NAME})

add_library(${TARGET_NAME} INTERFACE
        include/Platform.hpp
)
add_library(${PROJECT_NAME}::${LIBRARY_NAME} ALIAS ${TARGET_NAME})

target_include_directories(${TARGET_NAME}
        INTERFACE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

set_target_properties(${TARGET_NAME} PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}${LIBRARY_NAME}
        EXPORT_NAME ${LIBRARY_NAME}
)

# Tests

option(SHOX_ENABLE_TESTING "Enable testing of SHOX" ${PROJECT_IS_TOP_LEVEL})
if(SHOX_ENABLE_TESTING)
    add_subdirectory(tests)
endif()

if(NOT PROJECT_IS_TOP_LEVEL)
    # Users are unlikely to be interested in testing this
    # project, so don't show it in the basic options
    mark_as_advanced(SHOX_ENABLE_TESTING)
endif()

# Install and packaging

if(PROJECT_IS_TOP_LEVEL)
    add_subdirectory(install)
endif()