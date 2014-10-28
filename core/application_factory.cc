// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#include "core/application_factory.h"

#include "base/command_line.h"
#include "core/demo/flowdemo.h"
#include "core/demo/stereo_calib_demo.h"
#include "core/switches.h"

namespace core {

std::unique_ptr<Caroline> CreateApplication(base::CommandLine* command_line,
                                            Config* config) {
  if (command_line->HasSwitch(switches::kDemo)) {
    const std::string& demo = command_line->GetSwitchData(switches::kDemo);

    if (demo == demo::FlowDemo::kDemoName)
      return std::unique_ptr<Caroline>(
          new demo::FlowDemo(command_line, config));

    if (demo == demo::StereoCalibDemo::kDemoName)
      return std::unique_ptr<Caroline>(
          new demo::StereoCalibDemo(command_line, config));
  }
  return std::unique_ptr<Caroline>(new Caroline(command_line, config));
}

}  // namespace core

