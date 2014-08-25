# Copyright (c) 2014 The Caroline authors. All rights reserved.
# Use of this source file is governed by a MIT license that can be found in the
# LICENSE file.
# Author: Aleksandr Derbenev <13alexac@gmail.com>

cmake_minimum_required(VERSION 2.8)
project (Caroline CXX)

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
  set (default_use_own_opencv OFF)
else ()
  set (default_use_own_opencv ON)
endif ()

option(use_own_opencv
  "Own sources of opencv will be used on the non-linux systems."
  ${default_use_own_opencv})

list(APPEND CMAKE_MODULE_PATH "${Caroline_SOURCE_DIR}/cmake")
include(cxx11_support)
include(platform_utils)
include(targets_utils)

add_subdirectory(third_party)


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
include_directories("${OpenCV_INCLUDE_DIRS}")
include_directories("${Caroline_SOURCE_DIR}/third_party/jsoncpp/include")

include_directories("${Caroline_SOURCE_DIR}")
include_directories("${Caroline_BINARY_DIR}")

find_package(Protobuf REQUIRED)
include_directories(include_directories("${PROTOBUF_INCLUDE_DIRS}"))

list(APPEND Caroline_LIBRARIES
    ${OpenCV_LIBS}
    jsoncpp_lib
    ${PROTOBUF_LIBRARIES}
    )

protobuf_generate_cpp(PROTO_SOURCES PROTO_HEADERS
  "api/protocol.proto"
  )

file (COPY test_images DESTINATION ${Caroline_BINARY_DIR})

add_module(base OBJECT
  base/command_line.cc
  base/command_line.h
  base/logging.cc
  base/logging.h
  base/logging_posix.cc
  base/logging_win.cc
  base/stream.cc
  base/stream.h
  base/stream_net_posix.cc
  base/stream_net_posix.h
  base/stream_pipe_posix.cc
  base/stream_pipe_posix.h
  )

add_module(core OBJECT
  core/camera_calibration.cc
  core/camera_calibration.h
  core/cameras.cc
  core/cameras.h
  core/caroline.cc
  core/caroline.h
  core/config.cc
  core/config.h
  core/depth_map.h
  core/depth_map.cc
  core/depth_mesh.h
  core/depth_mesh.cc
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
  core/position_controller.cc
  core/position_controller.h
  core/position.cc
  core/position.h
  core/predefined_position_controller.cc
  core/predefined_position_controller.h
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
  core/time_controller.cc
  core/time_controller.h
  core/quaternion.cc
  core/quaternion.h
  core/undefined_position_controller.cc
  core/undefined_position_controller.h
  core/video_time_controller.cc
  core/video_time_controller.h
  ${PROTO_SOURCES}
  ${PROTO_HEADERS}
  )

add_application(caroline
  main.cc
  $<TARGET_OBJECTS:base>
  $<TARGET_OBJECTS:core>
  )
target_link_libraries(caroline
  ${Caroline_LIBRARIES}
  )

add_application(demo_flowdemo
  demo/flowdemo/main.cc
  demo/flowdemo/flowdemo.cc
  demo/flowdemo/flowdemo.h
  $<TARGET_OBJECTS:base>
  $<TARGET_OBJECTS:core>
  )

target_link_libraries(demo_flowdemo
  ${Caroline_LIBRARIES}
  )

add_application(demo_stereo_calib
  demo/calibratedemo/main.cc
  demo/calibratedemo/stereo_calib_demo.cc
  demo/calibratedemo/stereo_calib_demo.h
  $<TARGET_OBJECTS:base>
  $<TARGET_OBJECTS:core>
  )

target_link_libraries(demo_stereo_calib
  ${Caroline_LIBRARIES}
  )

enable_testing()
include_directories(third_party/gtest/include)
add_application(unit_tests
  base/stream_unittest.cc
  base/command_line_unittest.cc
  base/stream_unittest.cc
  core/cam_calib_unittest.cc
  core/cameras_unittest.cc
  core/scene_unittest.cc
  core/opencv_unittest.cc
  core/filter_unittest.cc
  core/recognise_unittest.cc
  $<TARGET_OBJECTS:base>
  $<TARGET_OBJECTS:core>
  )
target_link_libraries(unit_tests
  gtest_main
  ${Caroline_LIBRARIES}
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


