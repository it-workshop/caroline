# Copyright (c) 2014 The Caroline authors. All rights reserved.
# Use of this source file is governed by a MIT license that can be found in the
# LICENSE file.
# Author: Aleksandr Derbenev <13alexac@gmail.com>

cmake_minimum_required(VERSION 2.8)

function (init_platform)
  if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    set (OS_LINUX Y PARENT_SCOPE)
  elseif ("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
    set (OS_MACOSX Y PARENT_SCOPE)
  elseif ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    set (OS_WINDOWS Y PARENT_SCOPE)
  elseif ("${UNIX}")
    set (OS_POSIX Y PARENT_SCOPE)
  endif ()
endfunction()

function (platform_file_exclude_regexps regexps)
  set (list)

  if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    list (APPEND list "_notlinux.(h|cc)$")
  else ()
    list (APPEND list "_linux.(h|cc)$")
  endif ()

  if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
    list (APPEND list "_notmac.(h|cc|m|mm)$")
  else ()
    list (APPEND list "_mac.(h|cc|m|mm)")
  endif ()

  if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    list (APPEND list "_notwin.(h|cc)$")
  else ()
    list (APPEND list "_win.(h|cc)$")
  endif ()

  if ("${UNIX}")
    list (APPEND list "_notposix.(h|cc)$")
  else ()
    list (APPEND list "_posix.(h|cc)$")
  endif ()

  set ("${regexps}" "${list}" PARENT_SCOPE)
endfunction()

function (platform_definitions definitions)
  set (list)

  if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    list (APPEND list "OS_LINUX")
  endif ()

  if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
    list (APPEND list "OS_MACOSX")
  endif ()

  if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    list (APPEND list "OS_WINDOWS")
  endif ()

  if ("${UNIX}")
    list (APPEND list "OS_POSIX")
  endif ()

  set ("${definitions}" "${list}" PARENT_SCOPE)
endfunction ()

function (cxx11_compiler_flag flag)
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
      "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set ("${flag}" "-std=c++11" PARENT_SCOPE)
  endif ()
endfunction ()
