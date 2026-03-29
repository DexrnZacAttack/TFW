# TFW (Test Framework)
I wanted to experiment more with the STL and other concepts, so I made a TUI (+ headless) test framework.

Probably isn't the best out there, but I'm still proud of it.

## FetchContent (CMake)
```cmake
include(FetchContent)
FetchContent_Declare(tfw
        GIT_REPOSITORY https://codeberg.org/Dexrn/TFW
        GIT_TAG        main # or some other version if I update this
        GIT_PROGRESS   TRUE
        GIT_SHALLOW    TRUE
        EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(tfw)
```