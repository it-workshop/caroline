#!/usr/bin/env bash
# Copyright (c) 2014 The Caroline authors. All rights reserved.
# Use of this source file is governed by a MIT license that can be found in the
# LICENSE file.
# Author: Aleksandr Derbenev <13alexac@gmail.com>

set -e

svn checkout "http://googletest.googlecode.com/svn/trunk/" \
    $(pwd)/third_party/gtest
