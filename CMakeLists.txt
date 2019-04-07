cmake_minimum_required(VERSION 2.8.3)
project(zcm2ros)

cmake_policy(SET CMP0048 NEW)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_BUILD_TYPE "Release")

set(CMAKE_CXX_FLAGS "-std=c++11 -pthread")

if (PROFILE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
endif(PROFILE)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -ffast-math -ftree-vectorize -fopenmp -fprofile-generate -flto -fomit-frame-pointer")

set(CMAKE_C_FLAGS_MINSIZEREL ${CMAKE_CXX_FLAGS_RELEASE})

if (NOT ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64" OR ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86")
    #    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpu=neon -mfloat-abi=softfp")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv8-a+crypto -mcpu=cortex-a57+crypto")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv8-a+crypto -mcpu=cortex-a57+crypto")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -mfpmath=sse -mmmx -msse -msse2")
endif()

#execute_process(COMMAND src/zcm_types/zcm_gen.sh)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})
add_definitions(${OpenCV_DEFINITIONS})

#find_package(PCL REQUIRED)
#include_directories(${PCL_INCLUDE_DIRS})
#link_directories(${PCL_LIBRARY_DIRS})
#add_definitions(${PCL_DEFINITIONS})

find_package (GeographicLib REQUIRED)
add_definitions (${GeographicLib_DEFINITIONS})

find_package(
        catkin REQUIRED COMPONENTS
        roscpp
        rospy
        std_msgs
        message_generation
        network_interface
        roslint
        image_transport
        OpenCV
        cv_bridge
)

add_message_files(
        FILES
            RadarDetection.msg
            RadarDetectionStamped.msg
            RadarDetectionArray.msg
            RadarTrack.msg
            RadarTrackStamped.msg
            RadarTrackArray.msg
            # high level Tx
            RadarStatus.msg
            RadarErrorStatus.msg
            RadarInfo.msg
            RadarData.msg )

generate_messages(
        DEPENDENCIES std_msgs sensor_msgs geometry_msgs )

catkin_package(
        #INCLUDE_DIRS include
        LIBRARIES zcm2ros
        CATKIN_DEPENDS roscpp rospy std_msgs message_runtime network_interface geometry_msgs
        DEPENDS system_lib )

find_package(PkgConfig REQUIRED)
pkg_check_modules(ZCM REQUIRED zcm)
pkg_check_modules(ZMQ REQUIRED libzmq)
pkg_check_modules(MININI minini)

link_libraries(minIni)

include_directories(
        .
        ${catkin_INCLUDE_DIRS}
        /opt/Qt/5.9.1/gcc_64/include )

set(INC src/zcm_types/service/cpp_types/ZcmService.hpp
        src/zcm_types/lidar_scala/cpp_types/ZcmLidarScalaDataScan.hpp
        src/zcm_types/lidar_scala/cpp_types/ZcmLidarScalaPoint.hpp
        src/zcm_types/lidar_cluster/cpp_types/ZcmLidarCluster.hpp
        src/zcm_types/lidar_cluster/cpp_types/ZcmLidarClustersList.hpp
	)

set(SRC src/converter.cpp )


add_executable(zcm2ros_converter_node ${INC} ${SRC} )
target_link_libraries(zcm2ros_converter_node ${catkin_LIBRARIES} )
add_dependencies(zcm2ros_converter_node zcm2ros_generate_messages_cpp)
