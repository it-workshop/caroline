// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#include "base/command_line.h"
#include "base/logging.h"
#include "demo/flowdemo/flowdemo.h"
#include "core/config.h"
#include "core/switches.h"

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

  demo::FlowDemo application(command_line.get(), config.get());

  if (!application.Init()) {
    return -1;
  }

  return application.Run();
}
