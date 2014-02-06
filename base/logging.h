// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#ifndef BASE_LOGGING_H_
#define BASE_LOGGING_H_

#include <memory>
#include <string>

namespace base {

class Logger {
 public:
  enum class Level {
    NONE,
    ERR,
    WARNING,
    INFO,
    DEBUG
  };
  static const char kLevelError[];
  static const char kLevelWarning[];
  static const char kLevelInfo[];
  static const char kLevelDebug[];

  class Message {
   public:
    virtual ~Message();

    virtual Message& operator<<(int64_t i64);
    virtual Message& operator<<(uint64_t u64);
    virtual Message& operator<<(long double d);
    virtual Message& operator<<(char c);
    virtual Message& operator<<(void* ptr);
    virtual Message& operator<<(const std::string& str);
  };

  class NormalMessage : public Message {
   public:
    NormalMessage(std::shared_ptr<Logger> logger, const std::string& prefix,
        Level level);
    virtual ~NormalMessage();

    virtual Message& operator<<(int64_t i64) override;
    virtual Message& operator<<(uint64_t u64) override;
    virtual Message& operator<<(long double d) override;
    virtual Message& operator<<(char c) override;
    virtual Message& operator<<(void* ptr) override;
    virtual Message& operator<<(const std::string& str) override;

   private:
    std::string buffer_;
    std::shared_ptr<Logger> logger_;
    Level level_;
  };

  Logger();
  virtual ~Logger();

  static void Init(const std::string& file, Level minimum_level);
  static std::shared_ptr<Logger> GetInstance();

  Message AddMessage(Level level, const std::string& prefix);

  std::string file() const { return file_; }

 protected:
  virtual void PostMessage(const std::string& message) = 0;

 private:
  void InitInstance(const std::string& file, Level minimum_level);

  Level minimum_level_;
  std::string file_;

  Logger(const Logger&);
  Logger& operator=(const Logger&);
};

#define __LOG_PREFIX(level) \
    __FILE__  ":" __LINE__ ":" __func__ ":" #level ": "

// Use this macro if you want to log something.
// LOG(level) << "Hello!";
// Whele level is: ERR, WARNING, INFO, DEBUG.
#define LOG(level) \
    base::Logger::GetInstance()->AddMessage(Level::level, __LOG_PREFIX(level))

}  // namespace base

// Prevent shared pointer from instance in each file that uses logging.
extern template class std::shared_ptr<base::Logger>;

#endif  // BASE_LOGGING_H_
