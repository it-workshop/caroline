# Copyright (c) 2014 The Caroline authors. All rights reserved.
# Use of this source file is governed by a MIT license that can be found in the
# LICENSE file.
# Author: Aleksandr Derbenev <alex@technoworks.ru>

cmake_minimum_required(VERSION 2.8)

# CMake policies

cmake_policy(VERSION 2.8.0)

# Build options.

# use_own_opencv enables building opencv version from third_party/opencv
# directory. Disabled by default on linux.
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
  set (default_use_own_opencv OFF)
else ()
  set (default_use_own_opencv ON)
endif ()

option(use_own_opencv
  "Own sources of opencv will be used on the non-linux systems."
  ${default_use_own_opencv})
option(use_own_protobuf
  "Own sources of protobuf will be used by default."
  YES)

# Build project dependencies.
add_subdirectory(third_party)

# project section.
project (Caroline CXX)

# Reset CMAKE_*_OUTPUT_DIRECTORY after including third party projects.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${Caroline_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${Caroline_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${Caroline_BINARY_DIR}/bin")

# Enable utility cmake functions.
list(APPEND CMAKE_MODULE_PATH "${Caroline_SOURCE_DIR}/cmake")
include(cxx11_support)
include(platform_utils)
include(targets_utils)

if (use_own_protobuf)
  set (PROTOBUF_LIBRARY
    "${Protobuf_BINARY_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}protobuf${CMAKE_STATIC_LIBRARY_SUFFIX}"
    CACHE FILEPATH
    "The protobuf library"
    FORCE
    )
  set (PROTOBUF_INCLUDE_DIR
    "${Protobuf_SOURCE_DIR}/include"
    CACHE PATH
    "The include directory for protocol buffers"
    FORCE
    )
  set (PROTOBUF_LIBRARY_DEBUG
    "${Protobuf_BINARY_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}protobuf${CMAKE_STATIC_LIBRARY_SUFFIX}"
    CACHE FILEPATH
    "The protobuf library"
    FORCE
    )
  set (PROTOBUF_PROTOC_LIBRARY
    "${Protobuf_BINARY_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}protoc-library${CMAKE_STATIC_LIBRARY_SUFFIX}"
    CACHE FILEPATH
    "The protoc library"
    FORCE
    )
  set (PROTOBUF_PROTOC_LIBRARY_DEBUG
    "${Protobuf_BINARY_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}protoc-library${CMAKE_STATIC_LIBRARY_SUFFIX}"
    CACHE FILEPATH
    "The protoc library"
    FORCE
    )
  set (PROTOBUF_PROTOC_EXECUTABLE
    "${Protobuf_BINARY_DIR}/bin/protoc"
    CACHE FILEPATH
    "The protoc compiler"
    FORCE
    )
endif ()
find_package(Protobuf REQUIRED)

# Setup build options.
init_platform()
set (definitions)
platform_definitions(definitions)
foreach (definition IN LISTS definitions)
  add_definitions("-D${definition}")
endforeach ()
set (cxx11_flag)
cxx11_compiler_flag(cxx11_flag)
if (DEFINED cxx11_flag)
  add_definitions("${cxx11_flag}")
endif ()

# While using use_own_opencv option we must to enumerate explicitly used opencv
# modules.
if (NOT use_own_opencv)
  find_package(OpenCV REQUIRED)
else ()
  file (GLOB OpenCV_INCLUDE_DIRS
      "${Caroline_SOURCE_DIR}/third_party/opencv/modules/*/include")
  list (APPEND OpenCV_INCLUDE_DIRS
      "${Caroline_SOURCE_DIR}/third_party/opencv/include")
  set (OpenCV_LIBS
    opencv_core
    opencv_highgui
    opencv_imgproc
    opencv_video
    opencv_calib3d
    )
endif ()

# Set up include directories
include_directories(
  ${OpenCV_INCLUDE_DIRS}
  "${Caroline_SOURCE_DIR}/third_party/jsoncpp/include"
  ${PROTOBUF_INCLUDE_DIRS}
  "${Protobuf_SOURCE_DIR}/src"
  )

include_directories(
  "${Caroline_SOURCE_DIR}"
  "${Caroline_BINARY_DIR}"
  )

# enumerate third party libraries for linking.
list(APPEND Caroline_LIBRARIES
    ${OpenCV_LIBS}
    jsoncpp_lib
    ${PROTOBUF_LIBRARIES}
    )

# Generate files for protobuf seriallization/deseriallization.
protobuf_generate_cpp(PROTO_SOURCES PROTO_HEADERS
  "api/protocol.proto"
  )
set_source_files_properties(
    ${PROTO_SOURCES}
    ${PROTO_HEADERS}
    PROPERTIES
    GENERATED YES)

# Copy test images to the build directory.
file (COPY test_images DESTINATION ${Caroline_BINARY_DIR})

# API module. Must not depend from anything but protobuf.
add_module(api STATIC
  ${PROTO_HEADERS}
  ${PROTO_SOURCES}
  )
target_link_libraries(api
  ${PROTOBUF_LIBRARIES}
  )
add_dependencies(api protoc)

# base module. Infrastructure code that will be used in the project.
add_module(base STATIC
  base/at_exit.cc
  base/at_exit.h
  base/command_line.cc
  base/command_line.h
  base/logging.cc
  base/logging.h
  base/logging_posix.cc
  base/logging_win.cc
  base/path.cc
  base/path.h
  base/path_impl_posix.cc
  base/path_impl_posix.h
  base/path_service.cc
  base/path_service.h
  base/path_service_posix.cc
  base/path_service_posix.h
  base/stream.cc
  base/stream.h
  base/stream_net_posix.cc
  base/stream_net_posix.h
  base/stream_pipe_posix.cc
  base/stream_pipe_posix.h
  base/utils_posix.cc
  base/utils_posix.h
  )
target_link_libraries(base
  jsoncpp_lib
  )

# core module. Main code of the application.
add_module(core STATIC
  core/application_factory.cc
  core/application_factory.h
  core/camera_calibration.cc
  core/camera_calibration.h
  core/cameras.cc
  core/cameras.h
  core/caroline.cc
  core/caroline.h
  core/config.cc
  core/config.h
  core/demo/flowdemo.cc
  core/demo/flowdemo.h
  core/demo/stereo_calib_demo.cc
  core/demo/stereo_calib_demo.h
  core/depth_map.h
  core/depth_map.cc
  core/depth_mesh.h
  core/depth_mesh.cc
  core/device_manager.cc
  core/device_manager.h
  core/image_capture.cc
  core/image_capture.h
  core/image_capture_impl.cc
  core/image_capture_impl.h
  core/image_capture_manager.cc
  core/image_capture_manager.h
  core/image_capture_manager_impl.cc
  core/image_capture_manager_impl.h
  core/image_time_controller.cc
  core/image_time_controller.h
  core/lucas_kanade_optical_flow_processor.cc
  core/lucas_kanade_optical_flow_processor.h
  core/map_filter.h
  core/map_filter.cc
  core/map_recognise.h
  core/map_recognise.cc
  core/median_map_filter.h
  core/median_map_filter.cc
  core/mesh.h
  core/mesh.cc
  core/mesh_merge_utils.h
  core/mesh_merge_utils.cc
  core/metric.h
  core/metric.cc
  core/metric_factory.h
  core/metric_factory.cc
  core/metric_impls.h
  core/metric_impls.cc
  core/optical_flow.h
  core/optical_flow.cc
  core/optical_flow_processor.cc
  core/optical_flow_processor.h
  core/farneback_optical_flow_processor.cc
  core/farneback_optical_flow_processor.h
  core/ply_saver.h
  core/ply_saver.cc
  core/point_cloud.h
  core/point_cloud.cc
  core/position.cc
  core/position.h
  core/return_codes.h
  core/rotation_matrix.cc
  core/rotation_matrix.h
  core/scene3d.h
  core/scene3d.cc
  core/scene_element.h
  core/scene_element.cc
  core/serialization.cc
  core/serialization.h
  core/surface_mesh.h
  core/surface_mesh.cc
  core/switches.cc
  core/switches.h
  core/triangulation.cc
  core/triangulation.h
  core/triangulation_eigen.cc
  core/triangulation_eigen.h
  core/time_controller.cc
  core/time_controller.h
  core/quaternion.cc
  core/quaternion.h
  core/video_time_controller.cc
  core/video_time_controller.h
  )
target_link_libraries(core
  api
  base
  ${OpenCV_LIBS}
  ${PROTOBUF_LIBRARIES}
  jsoncpp_lib
  )
target_link_libraries(core api base)

add_application(caroline
  main.cc
  )
target_link_libraries(caroline
  ${Caroline_LIBRARIES}
  api
  base
  core
  )

enable_testing()
include_directories(third_party/gmock/include)
include_directories(third_party/gtest/include)
add_application(unit_tests
  unit_tests_main.cc
  base/at_exit_unittest.cc
  base/command_line_unittest.cc
  base/path_service_unittest_posix.cc
  base/stream_unittest.cc
  core/cam_calib_unittest.cc
  core/cameras_unittest.cc
  core/metric_unittest.cc
  core/scene_unittest.cc
  core/opencv_unittest.cc
  core/filter_unittest.cc
  core/recognise_unittest.cc
  )
target_link_libraries(unit_tests
  gmock
  gtest
  ${Caroline_LIBRARIES}
  api
  base
  core
  )
add_test(unit_tests unit_tests)

find_program(DOXYGEN doxygen)
if (NOT "${DOXYGEN}" STREQUAL "DOXYGEN-NOTFOUND")
  configure_file(
    "${Caroline_SOURCE_DIR}/Doxyfile.in"
    "${Caroline_BINARY_DIR}/Doxyfile"
    NEWLINE_STYLE UNIX)
  add_custom_target(docs
      "${DOXYGEN}" "${Caroline_BINARY_DIR}/Doxyfile"
      WORKING_DIRECTORY "${Caroline_BINARY_DIR}"
      COMMENT Build documentation.)
endif ()

# Packaging
set(CPACK_PACKAGE_NAME caroline)
set(CPACK_PACKAGE_VENDOR TechnoWorks)
set(CPACK_PACKAGE_VERSION_MAJOR 1)
set(CPACK_PACKAGE_VERSION_MINOR 0)
set(CPACK_PACKAGE_VERSION_PATCH 0)
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Computer vision system for robots.")
set(CPACK_PACKAGE_FILE_NAME
  "caroline-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}-unstable")
set(CPACK_PACKAGE_INSTALL_DIRECTORY
  "Caroline-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_OUTPUT_CONFIG_FILE "${Caroline_SOURCE_DIR}/CPackConfig.cmake")
set(CPACK_PACKAGE_EXECUTABLES
  demo_flowdemo "Caroline Optical flow demo"
  demo_stereo_calib "Caroline stereo cameras calibration demo"
  )
set(CPACK_STRIP_FILES YES)
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}")
set(CPACK_SOURCE_OUTPUT_CONFIG_FILE
  "${Caroline_BINARY_DIR}/CPackSourceConfig.cmake")
set(CPACK_PACKAGE_CONTACT "caroline@technoworks.ru")

if (${OS_WINDOWS})
  set(CPACK_NSIS_DISPLAY_NAME "Caroline")
  set(CPACK_NSIS_PACKAGE_NAME "Caroline")
  set(CPACK_NSIS_HELP_LINK "https://github.com/it-workshop/caroline/")
  set(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/it-workshop/caroline/")
  set(CPACK_NSIS_CONTACT "caroline@technoworks.ru")
endif ()

if (${OS_MACOSX})
  set(CPACK_OSX_PACKAGE_VERSION 10.6)
endif ()

if (${OS_POSIX})
  set(CPACK_DEBIAN_PACKAGE_NAME "caroline")
  set(CPACK_DEBIAN_PACKAGE_VERSION
    "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VESION_PATH}-unstable")
  set(deb_dependencies)
  set(rpm_dependencies)
  if (NOT ${use_own_opencv})
    list(APPEND deb_dependencies "libopencv")
    list(APPEND rpm_dependencies "opencv")
  endif ()
  if (NOT ${use_own_protobuf})
    list(APPEND deb_dependencies "libprotobuf7")
    list(APPEND rpm_dependencies "protobuf")
  endif ()
  set(CPACK_DEBIAN_PACKAGE_DEPENDS ${deb_dependencies})
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "alex@technoworks.ru")
  set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
  set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")

  set(CPACK_RPM_PACKAGE_RELEASE 1)
  set(CPACK_RPM_PACKAGE_LICENSE "MIT")
  set(CPACK_RPM_PACKAGE_VENDOR "TechnoWorks")
  set(CPACK_RPM_PACKAGE_REQUIRES ${rpm_dependencies})

  include(CPackDEB)
  include(CPackRPM)
endif ()

install(TARGETS
    caroline
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    )

include(CPack)
