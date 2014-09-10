#!/usr/bin/env python3
# Copyright (c) 2014 The Caroline authors. All rights reserved.
# Use of this source file is governed by a MIT license that can be found in the
# LICENSE file.
# Author: Aleksandr Derbenev <13alexac@gmail.com>

import fnmatch
import os
import re
from subprocess import call

__directory = os.path.dirname(os.path.abspath(__file__))
cpplint = os.path.join(__directory, 'cpplint', 'cpplint.py')

regexp = re.compile('^.+\.(?:c|cc|h|m|mm)$')

def search_sources_in_directory(name):
  ret = []
  if os.path.isdir(name):
    for item in os.listdir(name):
      if os.path.isfile(os.path.join(name, item)) and regexp.match(item):
        ret.append(os.path.join(name, item))
      elif os.path.isdir(os.path.join(name, item)):
        ret.extend(search_sources_in_directory(os.path.join(name, item)))
  elif os.path.isfile(name) and regexp.match(name):
    ret.append(name)
  return ret

directories = [
    'base',
    'core',
    'main.cc',
    'unit_tests_main.cc',
    'demo',
  ]

root = os.path.join(__directory, '..')

sources = []
for name in directories:
  sources.extend(search_sources_in_directory(os.path.join(root, name)))

command = ['python', cpplint, '--root=' + root]
command.extend(sources)
exit(call(command))

