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

include_directories("${CMAKE_SOURCE_DIR}")

add_module(base
  base/command_line.cc
  )

add_application(caroline
  main.cc
  )
target_link_libraries(caroline base)

enable_testing()
include_directories(third_party/gtest/include)
add_application(unit_tests
  unit_tests_main.cc
  )
target_link_libraries(unit_tests gtest)
add_test(unit_tests unit_tests)
