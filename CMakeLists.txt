cmake_minimum_required(VERSION 3.31)
project(tfw)

set(CMAKE_CXX_STANDARD 23)

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

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG        12.1.0
)
FetchContent_MakeAvailable(fmt)

add_subdirectory(lib/ftxui-extensions)

set(FILES
        include/TestFramework/test/result/TestResult.h
        include/TestFramework/test/ComparableTest.h
        src/test/util/ChronoWatch.cpp
        include/TestFramework/test/util/ChronoWatch.h
        include/TestFramework/test/util/TestOutputLogger.h
        src/TestFramework.cpp
        include/TestFramework/TestFramework.h
        include/TestFramework/test/exception/TestFailedException.h
        src/tui/Tui.cpp
        include/TestFramework/tui/Tui.h
        include/TestFramework/tui/IPostable.h
        include/TestFramework/tui/util/ColorUtil.h
        src/tui/components/TestButton.cpp
        include/TestFramework/tui/components/TestButton.h
        src/tui/components/TestListComponent.cpp
        include/TestFramework/tui/components/TestListComponent.h
        src/tui/components/TestStatusComponent.cpp
        include/TestFramework/tui/components/TestStatusComponent.h
)

add_subdirectory(tests)

add_library(tfw STATIC ${FILES})

target_include_directories(tfw
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(tfw
        PUBLIC fmt::fmt
        PUBLIC ftxui::screen
        PUBLIC ftxui::dom
        PUBLIC ftxui::component
        PUBLIC ftxui::extensions
)