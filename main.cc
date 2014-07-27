// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#include "base/command_line.h"
#include "base/logging.h"
#include "core/caroline.h"
#include "core/config.h"
#include "core/switches.h"
#include "core/return_codes.h"
#include "base/file_access.cc"

#include "base/stream_net_win.h"
int main(int argc, const char* argv[]) {
  /*
  int indicator;
  std::cout << "Enter 1 for Bind and 0 for Connect" << std::endl;
  std::cin >> indicator;
  if (indicator == 1){
    base::StreamNetWin stream;
    if (stream.base::StreamNetWin::ListenOnPort(3213) )
      std::cout << "Socket Binded";
  }
  else if (indicator == 0){
    base::StreamNetWin stream;
    if (stream.base::StreamNetWin::ConnectToHost(3213, "127.0.0.1"))
      std::cout << "Socket Connected";
  }
  */


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
      minimal_level = base::Logger::LOG_ERR;
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

  core::Caroline application(command_line.get(), config.get());

  if (!application.Init())
    return core::RETURN_APPLICATION_INIT_FAIL;
  return application.Run();
}
