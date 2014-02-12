# Copyright (c) 2014 The Caroline authors. All rights reserved.
# Use of this source file is governed by a MIT license that can be found in the
# LICENSE file.
# Author: Aleksandr Derbenev <13alexac@gmail.com>

cmake_minimum_required(VERSION 2.8.12)
project (Caroline CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
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

find_package(OpenCV REQUIRED)

include_directories("${OpenCV_INCLUDE_DIRS}")

include_directories("${CMAKE_SOURCE_DIR}")

add_module(base OBJECT
  base/command_line.cc
  base/command_line.h
  base/json.cc
  base/json.h
  base/logging.cc
  base/logging.h
  base/logging_posix.cc
  base/logging_win.cc
  base/values.cc
  base/values.h
  )


add_module(core OBJECT
  core/config.cc
  core/config.h
  core/switches.cc
  core/switches.h
  )

add_application(caroline
  main.cc
  $<TARGET_OBJECTS:base>
  $<TARGET_OBJECTS:core>
  )
target_link_libraries(caroline
  ${OpenCV_LIBS}
  )

enable_testing()
include_directories(third_party/gtest/include)
add_application(unit_tests
  base/command_line_unittest.cc
  base/json_unittest.cc
  base/values_unittest.cc
  core/opencv_unittest.cc
  unit_tests_main.cc
  $<TARGET_OBJECTS:base>
  $<TARGET_OBJECTS:core>
  )
target_link_libraries(unit_tests
  gtest
  ${OpenCV_LIBS}
  )
add_test(unit_tests unit_tests)
