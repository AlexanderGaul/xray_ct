cmake_minimum_required(VERSION 3.5.1)

# specifiy visualization source files
set(VIS_SRCS
        TransferFunction.cpp
        )
set(VIS_HDRS
        TransferFunction.h
        )

add_library(part2_lib_vis ${VIS_HDRS} ${VIS_SRCS} )
target_link_libraries(part2_lib_vis Qt5::Widgets)
