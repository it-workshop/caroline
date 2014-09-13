// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <13alexac@gmail.com>

#include "base/logging.h"

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

void Logger::AddObserver(Observer* observer) {
  observers_.insert(observer);
}

bool Logger::HasObserver(Observer* observer) const {
  return observers_.find(observer) != observers_.end();
}

void Logger::RemoveObserver(Observer* observer) {
  std::set<Observer*>::iterator it = observers_.find(observer);
  if (it != observers_.end())
  observers_.erase(it);
}

void Logger::InitInstance(std::shared_ptr<Logger> self,
    const std::string& file, Level minimum_level) {
  self_ = self;
  file_ = file;
  minimum_level_ = minimum_level;
}

void Logger::PostMessage(const std::string& message) {
  PostMessageImpl(message);
  for (auto observer : observers_)
    observer->Observe(message);
}

}  // namespace base

// Instance shared pointer here.
template class std::shared_ptr<base::Logger>;

