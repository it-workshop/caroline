// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author: Kukleva Anna <Kuklevaanna@gmail.com>


#include "base/command_line.h"
#include "base/logging.h"
#include "demo/calibratedemo/stereo_calib_demo.h"
#include "core/config.h"
#include "core/switches.h"


int main(int argc, const char* argv[]) {
    demo::StereoCalibDemo application;
    if (!application.Init()) {
      return -1;
    }
  return application.Run();
}
