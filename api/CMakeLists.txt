# Copyright (c) 2014 The Caroline authors. All rights reserved.
# Use of this source file is governed by a MIT license that can be found in the
# LICENSE file.
# Author: Vladislav Krikunov <vlad.krikunoff@yandex.ru> 

cmake_minimum_required(VERSION 2.8)

find_package(Protobuf REQUIRED)
include_directories(include_directories(${PROTOBUF_INCLUDE_DIRS})

PROTOBUF_GENERATE_CPP(PROTOCOL_SRCS PROTOCOL_HDRS protocol.proto)

