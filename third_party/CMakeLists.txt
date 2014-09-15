# Copyright (c) 2014 The Caroline authors. All rights reserved.
# Use of this source file is governed by a MIT license that can be found in the
# LICENSE file.
# Author: Aleksandr Derbenev <alex@technoworks.ru>

cmake_minimum_required(VERSION 2.8)

add_subdirectory(gmock)

if (use_own_opencv)
  if (MSVC)
    set (BUILD_WITH_STATIC_CRT OFF)
  endif ()
  add_subdirectory(opencv)
endif ()

add_subdirectory(jsoncpp)

if (use_own_protobuf)
  add_subdirectory(protobuf)
endif ()

