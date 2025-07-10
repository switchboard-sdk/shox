Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        2b60af89e23d28eefc081bc930831ee9d45ea58b # v3.8.1
)

FetchContent_MakeAvailable(Catch2)

add_executable(${TARGET_NAME}Tests
        Platform.test.cpp
)
target_link_libraries(${TARGET_NAME}Tests PRIVATE Catch2::Catch2WithMain)
target_include_directories(${TARGET_NAME}Tests
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(${TARGET_NAME}Tests)