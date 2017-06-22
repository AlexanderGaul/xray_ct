cmake_minimum_required(VERSION 3.5.1)

project("Part2")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

find_package(Eigen3 REQUIRED)
find_package(Qt5Widgets REQUIRED)

include_directories(${EIGEN3_INCLUDE_DIR} src/acq src/gui src)


IF (WIN32)
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
ELSE()
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
ENDIF()

# include catch test
set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test/catch)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

# specify acquisition source files
set(ACQ_SRCS
        src/acq/EDFhandler.cpp
        src/acq/AcquisitionModel.cpp
        src/acq/AcquisitionPose.cpp
        src/acq/RayTracing.cpp
        )
set(ACQ_HDRS
        src/acq/EDFhandler.h
        src/acq/IOutilities.h
        src/acq/Volume.h
        src/acq/AcquisitionModel.h
        src/acq/AcquisitionPose.h
        src/acq/RayTracing.h
        )

# specifiy reconstruction source files
set(REC_SRCS
        src/rec/CG.cpp
        src/rec/DataContainer.cpp
        )
set(REC_HDRS
        src/rec/CG.h
        src/rec/DataContainer.h
        )

# specify gui source files
set(GUI_SRCS 
        src/gui/MainWindow.cpp 
        src/gui/AcquisitionWidget.cpp
        src/gui/PoseViewer.cpp
        src/gui/PoseDisplay.cpp
        )
set(GUI_HDRS 
        src/gui/MainWindow.h 
        src/gui/AcquisitionWidget.h
        src/gui/PoseViewer.h
        src/gui/PoseDisplay.h
        )

set(PART2_TEST_SOURCES test/test_RayTracing.cpp)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
#only uncomment if automoc doesn't work
#qt5_wrap_cpp(moc_src include/DrawingWidget.h)

set(PART2_SOURCES ${ACQ_SRCS} ${REC_SRCS} ${GUI_SRCS})
set(PART2_HEADERS ${ACQ_HDRS} ${REC_HDRS} ${GUI_HDRS})

add_library(part2_lib ${PART2_HEADERS} ${PART2_SOURCES} )
target_link_libraries(part2_lib Qt5::Widgets)

# create executable for application
add_executable(part2 src/main.cpp )
set_target_properties(part2 PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(part2 part2_lib Qt5::Widgets)


#set(POSE_TEST test/acq_pose_tests.cpp)
#add_executable(pose_test ${POSE_TEST} src/catch.hpp)
#target_link_libraries(pose_test part2_lib Qt5::Widgets)

# create test executable
add_executable(part2_test ${PART2_TEST_SOURCES})
target_link_libraries(part2_test part2_lib Catch Qt5::Widgets)

