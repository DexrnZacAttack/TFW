cmake_minimum_required(VERSION 3.31)
project(tfw VERSION 1.1.0)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
set(FETCHCONTENT_QUIET OFF)

if (NOT TARGET ftxui::screen)
    FetchContent_Declare(ftxui
            GIT_REPOSITORY https://github.com/arthursonzogni/ftxui.git
            GIT_TAG        v6.1.9
            GIT_PROGRESS   TRUE
            GIT_SHALLOW    TRUE
            EXCLUDE_FROM_ALL
    )
    FetchContent_MakeAvailable(ftxui)
endif()

if (NOT TARGET fmt)
    FetchContent_Declare(
            fmt
            GIT_REPOSITORY https://github.com/fmtlib/fmt
            GIT_TAG        12.1.0
            GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(fmt)
endif()

if (NOT TARGET argum::argum)
    FetchContent_Declare(argum
            GIT_REPOSITORY https://github.com/gershnik/argum
            GIT_TAG v2.8
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(argum)
endif()

add_subdirectory(lib/ftxui-extensions)

set(FILES
        include/TestFramework/test/result/TestResult.h
        include/TestFramework/test/ComparableTest.h
        src/test/util/ChronoWatch.cpp
        include/TestFramework/test/util/ChronoWatch.h
        include/TestFramework/test/logging/loggers/ITestLogger.h
        src/TestFramework.cpp
        include/TestFramework/TestFramework.h
        include/TestFramework/test/exception/TestFailedException.h
        src/ui/tui/Tui.cpp
        include/TestFramework/ui/tui/TerminalUserInterface.h
        include/TestFramework/ui/IPostable.h
        include/TestFramework/ui/tui/util/ColorUtil.h
        src/ui/tui/components/TestButton.cpp
        include/TestFramework/ui/tui/components/TestButton.h
        src/ui/tui/components/TestListComponent.cpp
        include/TestFramework/ui/tui/components/TestListComponent.h
        src/ui/tui/components/TestStatusComponent.cpp
        include/TestFramework/ui/tui/components/TestStatusComponent.h
        include/TestFramework/ui/IUserInterface.h
        src/ui/IUserInterface.cpp
        src/ui/headless/HeadlessUserInterface.cpp
        include/TestFramework/ui/headless/HeadlessUserInterface.h
        include/TestFramework/ui/Event.h
        include/TestFramework/ui/EUserInterface.h
        src/ui/util/UserInterfaceFactory.cpp
        include/TestFramework/ui/util/UserInterfaceFactory.h
        include/TestFramework/test/logging/ITestLoggerProvider.h
        include/TestFramework/test/logging/loggers/ConsoleTestLogger.h
        include/TestFramework/test/logging/loggers/TuiTestLogger.h
        src/integration/ArgumIntegration.cpp
        include/TestFramework/integration/ArgumIntegration.h
)

add_subdirectory(tests)

add_library(tfw STATIC ${FILES})

target_include_directories(tfw
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(tfw
        PRIVATE argum::argum
        PUBLIC fmt::fmt
        PUBLIC ftxui::screen
        PUBLIC ftxui::dom
        PUBLIC ftxui::component
        PUBLIC ftxui::extensions
)