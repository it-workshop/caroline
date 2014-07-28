// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <13alexac@gmail.com>

#include <map>

#include "base/command_line.h"

namespace base {

class CommandLine::Impl {
 public:
  Impl();
  Impl(const Impl& impl);
  virtual ~Impl();

  bool HasSwitch(const std::string& key) const;
  std::string GetSwitchData(const std::string& key) const;
  void AddSwitch(const std::string& key, const std::string& data);
  void RemoveSwitch(const std::string& key);

 private:
  std::map<std::string, std::string> switches_;
};

CommandLine::Impl::Impl() {}

CommandLine::Impl::Impl(const Impl& impl)
  : switches_(impl.switches_.begin(), impl.switches_.end()) {}

CommandLine::Impl::~Impl() {}

bool CommandLine::Impl::HasSwitch(const std::string& key) const {
  return switches_.find(key) != switches_.end();
}

std::string CommandLine::Impl::GetSwitchData(const std::string& key) const {
  std::map<std::string, std::string>::const_iterator it =
      switches_.find(key);
  return it != switches_.end() ? it->second : std::string();
}

void CommandLine::Impl::AddSwitch(const std::string& key,
    const std::string& data) {
  switches_[key] = data;
}

void CommandLine::Impl::RemoveSwitch(const std::string& key) {
  std::map<std::string, std::string>::iterator it =
      switches_.find(key);
  if (it != switches_.end())
    switches_.erase(it);
}

// CommandLine

CommandLine::CommandLine()
  : impl_(new Impl()) {}

CommandLine::CommandLine(const char* argv[])
  : impl_(new Impl()) {
  ParseArgs(argv, this);
}

CommandLine::CommandLine(const CommandLine& command_line)
  : impl_(new Impl()) {
  *this = command_line;
}

CommandLine::~CommandLine() {}

CommandLine& CommandLine::operator=(const CommandLine& command_line) {
  if (command_line.impl_)
    std::unique_ptr<Impl>(new Impl(*command_line.impl_.get())).swap(impl_);
  else
    std::unique_ptr<Impl>(new Impl()).swap(impl_);
  return *this;
}

// static
void CommandLine::ParseArgs(const char* argv[], CommandLine* command_line) {
  auto impl = command_line->impl_.get();
  if (!impl)
    return;
  std::string last_key, arg_str;
  for (const char** arg = argv; *arg; ++arg) {
    arg_str = *arg;
    if (arg_str.size() >= 2 && '-' == arg_str[0] && '-' == arg_str[1]) {
      if (!last_key.empty())
        impl->AddSwitch(last_key, std::string());
      arg_str = arg_str.substr(2);
      last_key = arg_str;
    } else {
      impl->AddSwitch(last_key, arg_str);
      last_key = "";
    }
    if (!*(arg + 1) && !last_key.empty())
      impl->AddSwitch(last_key, std::string());
  }
}

// static
std::shared_ptr<CommandLine> CommandLine::GetForCurrentProcess() {
  static std::shared_ptr<CommandLine> command_line(new CommandLine());
  return command_line;
}

bool CommandLine::HasSwitch(const std::string& key) const {
  return impl_ && impl_->HasSwitch(key);
}

std::string CommandLine::GetSwitchData(const std::string& key) const {
  return impl_ ? impl_->GetSwitchData(key) : std::string();
}

void CommandLine::AddSwitch(const std::string& key) {
  if (impl_)
    impl_->AddSwitch(key, std::string());
}

void CommandLine::AddSwitchWithData(const std::string& key,
    const std::string& data) {
  if (impl_)
    impl_->AddSwitch(key, data);
}

void CommandLine::RemoveSwitch(const std::string& key) {
  if (impl_)
    impl_->RemoveSwitch(key);
}

}  // namespace base
