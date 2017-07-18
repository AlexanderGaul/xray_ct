cmake_minimum_required(VERSION 3.5.1)

project("Part2")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

find_package(Eigen3 REQUIRED)
find_package(Qt5Widgets REQUIRED)

include_directories(${EIGEN3_INCLUDE_DIR}
    src/util
    src/acq
    src/rec
    src/vis
    src/gui
)


IF (WIN32)
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
ELSE()
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
ENDIF()

# include catch test
set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test/catch)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
#only uncomment if automoc doesn't work
#qt5_wrap_cpp(moc_src include/DrawingWidget.h)

add_subdirectory(src)

# create executable for application
add_executable(part2 src/main.cpp)
set_target_properties(part2 PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(part2
                            part2_lib_util
                            part2_lib_acq
                            part2_lib_rec
                            part2_lib_vis
                            part2_lib_gui
                            Qt5::Widgets)

#set(POSE_TEST test/acq_pose_tests.cpp)
#add_executable(pose_test ${POSE_TEST} src/catch.hpp)
#target_link_libraries(pose_test part2_lib Qt5::Widgets)

# specify test source files
set(PART2_TEST_SOURCES
        test/test_CG.cpp
        test/test_MIP.cpp
        test/test_RayTracing.cpp
        test/test_SinogramGeneration.cpp
        test/test_Volume.cpp
        )
# create test executable
add_executable(part2_test ${PART2_TEST_SOURCES})
target_link_libraries(part2_test
                            part2_lib_util
                            part2_lib_acq
                            part2_lib_rec
                            part2_lib_gui
                            part2_lib_vis
                            Qt5::Widgets
                            Catch)

