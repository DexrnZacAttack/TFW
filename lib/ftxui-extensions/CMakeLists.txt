cmake_minimum_required(VERSION 3.31)
project(ftxui-extensions)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/arthursonzogni/ftxui.git
        GIT_TAG        v6.1.9
        GIT_PROGRESS   TRUE
        GIT_SHALLOW    TRUE
        EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(ftxui)

set(FILES
        src/Scroller.cpp
        include/ftxui/Scroller.h
)

add_library(ftxui-extensions STATIC ${FILES})
add_library(ftxui::extensions ALIAS ftxui-extensions)

target_include_directories(ftxui-extensions
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(ftxui-extensions
        PUBLIC ftxui::screen
        PUBLIC ftxui::dom
        PUBLIC ftxui::component
)