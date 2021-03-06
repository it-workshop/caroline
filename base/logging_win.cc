// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#include <Windows.h>

#include <iostream>  // NOLINT
#include <string>

#include "base/logging.h"

namespace base {

/// Logger implementation for windows.
class LoggerWin : public Logger {
 public:
  /// Default constructor.
  LoggerWin();
  /// Destructor.
  virtual ~LoggerWin();

  /// Implementation of Stream::PostMessage().
  /// @param[in] message Message to write into the log.
  virtual void PostMessageImpl(const std::string& message) override;

 private:
  /// Copy constuctor is disallowed.
  LoggerWin(const LoggerWin&);
  /// Assign operator is disallowed.
  LoggerWin& operator=(const LoggerWin&);
};

// static
std::shared_ptr<Logger> Logger::GetInstance() {
  static std::shared_ptr<Logger> instance(new LoggerWin());
  return instance;
}

LoggerWin::LoggerWin() {}

LoggerWin::~LoggerWin() {}

void LoggerWin::PostMessageImpl(const std::string& message) {
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
