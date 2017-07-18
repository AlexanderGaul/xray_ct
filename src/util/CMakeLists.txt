cmake_minimum_required(VERSION 3.5.1)

# specifiy reconstruction source files
set(UTIL_SRCS
        Timer.cpp
        )
set(UTIL_HDRS
        Timer.h
        Acquisition.h
        DebugStuff.h
        )

add_library(part2_lib_util ${UTIL_HDRS} ${UTIL_SRCS} )
target_link_libraries(part2_lib_util Qt5::Widgets)
