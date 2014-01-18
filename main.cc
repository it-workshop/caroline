// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#include "base/command_line.h"
#include "core/config.h"
#include "core/switches.h"

int main(int argc, const char* argv[]) {
  auto command_line(base::CommandLine::GetForCurrentProcess());
  base::CommandLine::ParseArgs(argv, command_line.get());

  if (command_line->HasSwitch(core::switches::kConfigSwitch)) {
    auto config = core::Config::GetInstance();
    config->LoadFromFile(
        command_line->GetSwitchData(core::switches::kConfigSwitch));
  }

  return 0;
}
