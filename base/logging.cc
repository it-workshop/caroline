// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#include "base/logging.h"

namespace base {

const char Logger::kLevelError[] = "error";
const char Logger::kLevelWarning[] = "warning";
const char Logger::kLevelInfo[] = "info";
const char Logger::kLevelDebug[] = "debug";

Logger::Message::~Message() {}

Logger::Message& Logger::Message::operator<<(int64_t i64) { return *this; }
Logger::Message& Logger::Message::operator<<(uint64_t u64) { return *this; }
Logger::Message& Logger::Message::operator<<(long double ld) { return *this; }
Logger::Message& Logger::Message::operator<<(char c) { return *this; }
Logger::Message& Logger::Message::operator<<(void* ptr) { return *this; }
Logger::Message& Logger::Message::operator<<(const std::string& str) {
  return *this;
}

Logger::NormalMessage::NormalMessage(std::shared_ptr<Logger> logger,
    const std::string& prefix, Level level)
  : logger_(logger),
    buffer_(prefix),
    level_(level) {}

Logger::NormalMessage::~NormalMessage() {
  buffer_.append("\n");
  logger_->PostMessage(buffer_);
}

Logger::Message& Logger::NormalMessage::operator<<(int64_t i64) {
  buffer_.append(std::to_string(i64));
  return *this;
}

Logger::Message& Logger::NormalMessage::operator<<(uint64_t u64) {
  buffer_.append(std::to_string(u64));
  return *this;
}

Logger::Message& Logger::NormalMessage::operator<<(long double ld) {
  buffer_.append(std::to_string(ld));
  return *this;
}

Logger::Message& Logger::NormalMessage::operator<<(char c) {
  buffer_.append(1, c);
  return *this;
}

Logger::Message& Logger::NormalMessage::operator<<(void* ptr) {
  const char hex_alphabet[] = "0123456789ABCDEF";
  buffer_.append("0x");
  for (int i = sizeof(ptr) * 8 - 4; i >= 0; i -= 4)
    buffer_.append(1,
        hex_alphabet[(reinterpret_cast<size_t>(ptr) >> i) & 0x0f]);
  return *this;
}

Logger::Message& Logger::NormalMessage::operator<<(const std::string& str) {
  buffer_.append(str);
  return *this;
}

Logger::Logger()
  : minimum_level_(Level::NONE) {}

Logger::~Logger() {}

// static
void Logger::Init(const std::string& file, Level minimum_level) {
  GetInstance()->InitInstance(file, minimum_level);
}

Logger::Message Logger::AddMessage(Level level, const std::string& prefix) {
  if (level > minimum_level_)
    return Message();
  return NormalMessage(GetInstance(), prefix, level);
}

void Logger::InitInstance(const std::string& file, Level minimum_level) {
  file_ = file;
  minimum_level_ = minimum_level;
}

}  // namespace base

// Instance shared pointer here.
template class std::shared_ptr<base::Logger>;
