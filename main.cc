// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#include "base/command_line.h"
#include "base/logging.h"
#include "core/config.h"
#include "core/image_capture_manager.h"
#include "core/switches.h"
#include "core/time_controller.h"

int main(int argc, const char* argv[]) {
  auto command_line(base::CommandLine::GetForCurrentProcess());
  base::CommandLine::ParseArgs(argv, command_line.get());

  auto config = core::Config::GetInstance();
  if (command_line->HasSwitch(core::switches::kConfigSwitch)) {
    config->LoadFromFile(
        command_line->GetSwitchData(core::switches::kConfigSwitch));
  }

  if (command_line->HasSwitch(core::switches::kEnableLogging)) {
    std::string level =
        command_line->GetSwitchData(core::switches::kEnableLogging);
    base::Logger::Level minimal_level = base::Logger::LOG_NONE;
    if (base::Logger::kLevelError == level)
      minimal_level = base::Logger::LOG_ERROR;
    else if (base::Logger::kLevelWarning == level)
      minimal_level = base::Logger::LOG_WARNING;
    else if (base::Logger::kLevelInfo == level)
      minimal_level = base::Logger::LOG_INFO;
    else if (base::Logger::kLevelDebug == level)
      minimal_level = base::Logger::LOG_DEBUG;
    std::string file =
        command_line->GetSwitchData(core::switches::kLogFile);
    base::Logger::GetInstance()->Init(file, minimal_level);
  }

  auto image_capture_manager(core::ImageCaptureManager::Create(config.get()));
  if (!image_capture_manager || image_capture_manager->GetCapturesCount() < 2)
    return -1;

  // auto captures = image_capture_manager->GetCaptures();
  // for (auto capture : captures)
  //   double focus_length_in_px =
  //       capture->GetFocusLength() * capture->GetDpm()
  // TODO(VladimirGl): Add Cameras initialisation here.

  while (image_capture_manager->GetTimeController()->Grab()) {
    // auto frameset = image_capture_manager->GetNextFrameset();
    // TODO(VladimirGl): Use frameset here to build depth map.
  }

  return 0;
}
