// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "demo/sgbmdemo/sgbm_depth_demo.h"

int main() {
  demo::SGBMDepthDemo application;

  if (!application.Init()) {
    return -1;
  }

  return application.Run();
}
