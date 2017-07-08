cmake_minimum_required(VERSION 3.5.1)

# specify acquisition source files
set(ACQ_SRCS
        EDFhandler.cpp
        AcquisitionModel.cpp
        AcquisitionPose.cpp
        RayTracing.cpp
        ForwardProjectionOperator.cpp
        )
set(ACQ_HDRS
        EDFhandler.h
        IOutilities.h
        Volume.h
        AcquisitionModel.h
        AcquisitionPose.h
        RayTracing.h
        Vec3D.h
        ForwardProjectionOperator.h
        definitions.h
        )

add_library(part2_lib_acq ${ACQ_HDRS} ${ACQ_SRCS} )
target_link_libraries(part2_lib_acq Qt5::Widgets)
