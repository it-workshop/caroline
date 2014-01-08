# Copyright (c) 2014 The Caroline authors. All rights reserved.
# Use of this source file is governed by a MIT license that can be found in the
# LICENSE file.
# Author: Aleksandr Derbenev <13alexac@gmail.com>

cmake_minimum_required(VERSION 2.8.12)

include (platform_utils)

function (filter_sources out in)
  set (sources)
  set (regexps)
  platform_file_exclude_regexps(regexps)
  foreach (file IN LISTS in)
    set (matched N)
    foreach (regexp IN LISTS regexps)
      if ("${file}" MATCHES "${regexp}")
        set (matched Y)
        break ()
      endif ()
    endforeach ()
    if (NOT "${matched}")
      list (APPEND sources "${file}")
    endif ()
  endforeach ()
  set ("${out}" "${sources}" PARENT_SCOPE)
endfunction ()

function (add_application name)
  set (sources)
  filter_sources(sources "${ARGN}")
  add_executable ("${name}" "${sources}")
endfunction ()

function (add_module name)
  set (sources)
  filter_sources(sources "${ARGN}")
  add_library ("${name}" "${sources}")
endfunction ()