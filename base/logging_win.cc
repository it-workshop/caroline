// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#include <Windows.h>

#include <iostream>  // NOLINT
#include <string>

#include "base/logging.h"

namespace base {

class LoggerWin : public Logger {
 public:
  LoggerWin();
  virtual ~LoggerWin();

  virtual void PostMessage(const std::string& message) override;

 private:
  LoggerWin(const LoggerWin&);
  LoggerWin& operator=(const LoggerWin&);
};

// static
std::shared_ptr<Logger> Logger::GetInstance() {
  static std::shared_ptr<Logger> instance(new LoggerWin());
  return instance;
}

LoggerWin::LoggerWin() {}

LoggerWin::~LoggerWin() {}

void LoggerWin::PostMessage(const std::string& message) {
  std::cout << message << std::flush;
  if (file().empty())
    return;

  HANDLE hfile = CreateFile(file().c_str(), GENERIC_WRITE, 0, nullptr,
      OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
  if (INVALID_HANDLE_VALUE == hfile &&
      GetLastError() != ERROR_SHARING_VIOLATION)
    return;

  if (!hfile) {
    // Windows can't lock file and freeze process that tries to open it.
    // So just sleep for 10 ms and try again if file is locked.
    Sleep(10);
    hfile = CreateFile(file().c_str(), GENERIC_WRITE, 0, nullptr, OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, nullptr);
  }

  if (!hfile)
    return;

  if (SetFilePointer(hfile, 0, nullptr, FILE_END) == INVALID_SET_FILE_POINTER) {
    CloseHandle(hfile);
    return;
  }

  // We can't do anything here. Just ignore error if it occurs.
  WriteFile(hfile, message.c_str(), message.size(), nullptr, nullptr);
  CloseHandle(hfile);
}

}  // namespace base
