// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <13alexac@gmail.com>

#ifndef BASE_COMMAND_LINE_H_
#define BASE_COMMAND_LINE_H_

#include <memory>
#include <string>

namespace base {

/// Class represents command line of program.
///
/// Each switch is expected to start with "--"
/// Each switch has a data string (possibly empty).
class CommandLine {
 public:
  /// Construct an object from standard main() arguments.
  /// @param[in] argv Standard main() arguments.
  explicit CommandLine(const char* argv[]);
  /// Copy constructor.
  /// @param[in] command_line Object to copy.
  CommandLine(const CommandLine& command_line);
  /// Default constructor.
  CommandLine();
  /// Destructor.
  virtual ~CommandLine();

  /// Assign operator.
  /// @param[in] command_line Object to copy.
  /// @returns non-constant reference to this object.
  CommandLine& operator=(const CommandLine& command_line);

  /// Parse arguments and fill in the object.
  /// @param[in]  argv         standard main() arguments.
  /// @param[out] command_line object to fill in.
  static void ParseArgs(const char* argv[], CommandLine* command_line);

  /// Singleton instance.
  ///
  /// This object is initialized by default constructor. You must to use
  /// ParseArgs to fill it.
  ///
  /// @return pointer to the singleton object.
  static std::shared_ptr<CommandLine> GetForCurrentProcess();

  /// Checks that argument exists.
  /// @param[in] key Argument without "--".
  /// @returns true if argument exists.
  bool HasSwitch(const std::string& key) const;
  /// Argument parameter.
  /// @param[in] key Argument without "--".
  /// @returns string associated with given switch.
  std::string GetSwitchData(const std::string& key) const;
  /// Adds switch without parameter.
  /// @param[in] key Argument without "--".
  void AddSwitch(const std::string& key);
  /// Adds switch with string parameter.
  /// @param[in] key  Argument without "--".
  /// @param[in] data String parameter.
  void AddSwitchWithData(const std::string& key, const std::string& data);
  /// Deletes given argument.
  /// @param[in] key Argument without "--".
  void RemoveSwitch(const std::string& key);

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace base

#endif  // BASE_COMMAND_LINE_H_
