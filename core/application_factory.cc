// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#include "core/application_factory.h"

#include <string>

#include "base/command_line.h"
#include "base/path_service.h"
#include "core/demo/flowdemo.h"
#include "core/demo/stereo_calib_demo.h"
#include "core/optical_flow_processor.h"
#include "core/preferences_service.h"
#include "core/switches.h"

namespace core {

std::unique_ptr<Caroline> CreateApplication(base::CommandLine* command_line) {
  if (command_line->HasSwitch(switches::kDemo)) {
    const std::string& demo = command_line->GetSwitchData(switches::kDemo);
    if (demo == demo::FlowDemo::kDemoName)
      OpticalFlowProcessor::RegisterPreferences();
      if (command_line->HasSwitch(core::switches::kConfigSwitch)) {
        PrefService* prefs = PrefService::GetInstance();
        prefs->LoadFromConfig(base::Path(
          command_line->GetSwitchData(core::switches::kConfigSwitch)).spec());
      }
      return std::unique_ptr<Caroline>(
          new demo::FlowDemo(command_line));

    if (demo == demo::StereoCalibDemo::kDemoName)
      return std::unique_ptr<Caroline>(
          new demo::StereoCalibDemo(command_line));
  }
  return std::unique_ptr<Caroline>(new Caroline(command_line));
}

}  // namespace core

