// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#include "base/utils_posix.h"

#include <errno.h>
#include <cstdio>

namespace base {

std::string GetError() {
  int err = errno;
  if (err > sys_nerr)
    return sys_errlist[EINVAL];
  return std::string(sys_errlist[err]);
}

}  // namespace base
