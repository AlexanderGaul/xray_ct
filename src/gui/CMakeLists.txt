cmake_minimum_required(VERSION 3.5.1)

# specify gui source files
set(GUI_SRCS
        MainWindow.cpp
        AcquisitionWidget.cpp
        PoseViewer.cpp
        PoseDisplay.cpp
        ResultWidget.cpp
        )
set(GUI_HDRS
        MainWindow.h
        AcquisitionWidget.h
        PoseViewer.h
        PoseDisplay.h
        ResultWidget.h
        )

add_library(part2_lib_gui ${GUI_HDRS} ${GUI_SRCS} )
target_link_libraries(part2_lib_gui Qt5::Widgets)
