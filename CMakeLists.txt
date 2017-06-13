cmake_minimum_required(VERSION 3.5.1)

project("Part2")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

find_package(Eigen3 REQUIRED)
find_package(Qt5Widgets REQUIRED)

include_directories(${EIGEN3_INCLUDE_DIR} src/acq src/gui)

IF (WIN32)
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
ELSE()
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
ENDIF()


set(ACQ_SRCS
        src/acq/EDFhandler.cpp
        src/acq/AcquisitionModel.cpp
        src/acq/AcquisitionPose.cpp)
set(ACQ_HDRS
        src/acq/EDFhandler.h
        src/acq/IOutilities.h
        src/acq/Volume.h
        src/acq/AcquisitionModel.h
        src/acq/AcquisitionPose.h
        )

set(GUI_SRCS src/gui/MainWindow.cpp src/gui/AcquisitionWidget.cpp)
set(GUI_HDRS src/gui/MainWindow.h src/gui/MainWindow.h)
#set(PART2_TEST_SRCS )

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
#only uncomment if automoc doesn't work
#qt5_wrap_cpp(moc_src include/DrawingWidget.h)

set(PART2_SOURCES src/main.cpp ${ACQ_SRCS} ${GUI_SRCS})
set(PART2_HEADERS ${ACQ_HDRS} ${GUI_HDRS})

add_executable(part2 ${PART2_HEADERS} ${PART2_SOURCES} )
set_target_properties(part2 PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(part2 Qt5::Widgets)
#add_executable(part2_tests ${PART2_TEST_SRCS} ${PART2_HEADERS} src/catch.hpp)
#target_link_libraries(part2_tests Qt5::Widgets)
