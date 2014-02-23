# Copyright (c) 2014 The Caroline authors. All rights reserved.
# Use of this source file is governed by a MIT license that can be found in the
# LICENSE file.
# Author: Aleksandr Derbenev <13alexac@gmail.com>

cmake_minimum_required(VERSION 2.8.12)

add_subdirectory(gtest)

if (use_own_opencv)
  add_subdirectory(opencv)
endif ()
