// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#ifndef BASE_COMMAND_LINE_H_
#define BASE_COMMAND_LINE_H_

#include <memory>
#include <string>

namespace base {

// Class represents command line of program.
// Each switch is expected to start with "--"
// Each switch has a data string (possibly empty).
class CommandLine {
 public:
  explicit CommandLine(const char* argv[]);
  CommandLine(const CommandLine& command_line);
  CommandLine();
  virtual ~CommandLine();

  CommandLine& operator=(const CommandLine& command_line);

  static void ParseArgs(const char* argv[], CommandLine* command_line);
  // You don't own object retrived by Get method. Do not release it!
  static std::shared_ptr<CommandLine> GetForCurrentProcess();

  bool HasSwitch(const std::string& key) const;
  std::string GetSwitchData(const std::string& key) const;
  void AddSwitch(const std::string& key);
  void AddSwitchWithData(const std::string& key, const std::string& data);
  void RemoveSwitch(const std::string& key);

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace base

#endif  // BASE_COMMAND_LINE_H_
