// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// Author: Aleksandr Derbenev <13alexac@gmail.com>

#include "base/at_exit.h"
#include "base/command_line.h"
#include "base/logging.h"
#include "base/path_service.h"
#include "core/application_factory.h"
#include "core/caroline.h"
#include "core/preferences_service.h"
#include "core/switches.h"
#include "core/return_codes.h"

/// Entry point of the program.
/// @param argc Number of command line arguments.
/// @param argv nullptr terminated list of command line arguments.
/// @returns 0 on success.
int main(int argc, const char* argv[]) {
  base::AtExitManager at_exit_manager;
  base::PathService::Init(*argv);

  auto command_line(base::CommandLine::GetForCurrentProcess());
  base::CommandLine::ParseArgs(argv, command_line.get());

  core::PrefService::Init();

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
    base::Logger::Init(file, minimal_level);
  }

  std::unique_ptr<core::Caroline> application(
      core::CreateApplication(command_line.get()));

  if (!application->Init())
    return core::RETURN_APPLICATION_INIT_FAIL;

  return application->Run();
}
