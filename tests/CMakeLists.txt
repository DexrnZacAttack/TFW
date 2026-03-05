cmake_minimum_required(VERSION 3.31)
project(tfw-tests)

set(CMAKE_CXX_STANDARD 23)

set(TEST_FILES
    src/Tests.cpp
)

add_executable(tfw-tests ${TEST_FILES})

target_include_directories(tfw-tests
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(tfw-tests
        PRIVATE tfw
)