#!/usr/bin/env bash
# Copyright (c) 2014 The Caroline authors. All rights reserved.
# Use of this source file is governed by a MIT license that can be found in the
# LICENSE file.
# Author: Aleksandr Derbenev <13alexac@gmail.com>

sources="$(find $1 -regex '.*\.cc')"
headers="$(find $1 -regex '.*\.h')"
objcsources="$(find $1 -regex '.*\.m')"
objcxxsources="$(find $1 -regex '.*\.mm')"

sources_to_lint="${sources} ${headers} ${objcsources} ${objcxxsources}"

function exclude {
  sources_to_lint=$(echo ${sources_to_lint} | sed -e 's/ /\
/g' | grep -v $1)
}

exclude third_party
exclude cpplint

$1/build/cpplint/cpplint.py --root=$1 --extensions=h,cc,m,mm \
    ${sources_to_lint}
