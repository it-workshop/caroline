// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <13alexac@gmail.com>

#include "base/logging.h"

#include "core/serialization.h"

namespace base {

const char Logger::kLevelError[] = "error";
const char Logger::kLevelWarning[] = "warning";
const char Logger::kLevelInfo[] = "info";
const char Logger::kLevelDebug[] = "debug";

Logger::Message::Message(std::shared_ptr<Logger> logger, bool visible)
  : logger_(std::move(logger)),
    visible_(visible),
    stream_(new std::ostringstream()) {}

Logger::Message::~Message() {
  if (visible_ && !stream_->str().empty()) {
    *stream_ << '\n';
    logger_->PostMessage(stream_->str());
  }
}

Logger::Logger()
  : minimum_level_(Level::LOG_NONE) {}

Logger::~Logger() {}

// static
void Logger::Init(const std::string& file, Level minimum_level) {
  std::shared_ptr<Logger> instance = GetInstance();
  instance->InitInstance(instance, file, minimum_level);
}

Logger::Message Logger::AddMessage(Level level) {
  return Message(self(), level <= minimum_level_);
}

void Logger::InitInstance(std::shared_ptr<Logger> self,
    const std::string& file, Level minimum_level) {
  self_ = self;
  file_ = file;
  minimum_level_ = minimum_level;
}

void Logger::PostMessage(const std::string& message) {
  bitdata::GlobalMessage output;
  output.SetStream(connection_address_);
  output.GenLog(message);
  PostMessageImpl(message);
}

}  // namespace base

// Instance shared pointer here.
template class std::shared_ptr<base::Logger>;

