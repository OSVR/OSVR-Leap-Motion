cmake_minimum_required(VERSION 2.8.12)
project(LeapMotionPlugin) # Change this line.

# This looks for an osvrConfig.cmake file - most of the time it can be
# autodetected but you might need to create/extend CMAKE_PREFIX_PATH to include something like
# C:/Users/Ryan/Desktop/build/OSVR-Core-vc12 or
# C:/Users/Ryan/Downloads/OSVR-Core-Snapshot-v0.1-406-gaa55515-build54-vs12-32bit
# in the CMake GUI or command line.
find_package(osvr)

# This generates a header file, from the named json file, containing a string literal
# named com_osvr_LeapMotion_json (not null terminated)
# The file must be added as a source file to some target (as below) to be generated.
osvr_convert_json(com_osvr_LeapMotion_json
    com_osvr_LeapMotion.json
    "${CMAKE_CURRENT_BINARY_DIR}/com_osvr_LeapMotion_json.h")

# Be able to find our generated header file.
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

include_directories(com_osvr_LeapMotion
	${LEAP_SDK_PATH}/include
	${OPENCV_INCLUDE_DIR}
	${EIGEN3_INCLUDE_DIR})

# This is just a helper function wrapping CMake's add_library command that
# sets up include dirs, libraries, and naming convention (no leading "lib")
# for an OSVR plugin. It also installs the plugin into the right directory.
# Pass as many source files as you need. See osvrAddPlugin.cmake for full docs.
osvr_add_plugin(NAME com_osvr_LeapMotion
    CPP # indicates we'd like to use the C++ wrapper
    SOURCES
	Analog.cpp
	Analog.h
	Configure.cpp
	Configure.h
    com_osvr_LeapMotion.cpp
	ControllerDevice.cpp
	ControllerDevice.h
	Gesture.cpp
	Gesture.h
	Imaging.cpp
	Imaging.h
	Tracker.cpp
	Tracker.h
    "${CMAKE_CURRENT_BINARY_DIR}/com_osvr_LeapMotion_json.h")

# If you use other libraries, find them and add a line like:
target_link_libraries(com_osvr_LeapMotion
	${LEAP_SDK_PATH}/lib/x86/Leap.lib
	${OpenCV_DIR}/opencv_core2410.lib)

add_custom_command(
	TARGET com_osvr_LeapMotion
	POST_BUILD
	COMMAND cp $(TargetPath) ${CMAKE_PREFIX_PATH}/bin/osvr-plugins-0/com_osvr_LeapMotion.dll
	COMMENT "Copying plugin build to OSVR Server..."
)
