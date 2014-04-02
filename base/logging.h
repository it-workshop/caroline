// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#ifndef BASE_LOGGING_H_
#define BASE_LOGGING_H_

#include <memory>
#include <sstream>
#include <string>

namespace base {

class Logger {
 public:
  enum Level {
    LOG_NONE,
    LOG_ERROR,
    LOG_WARNING,
    LOG_INFO,
    LOG_DEBUG
  };
  static const char kLevelError[];
  static const char kLevelWarning[];
  static const char kLevelInfo[];
  static const char kLevelDebug[];

  class Message {
   public:
    Message(std::shared_ptr<Logger> logger, bool visible);
    virtual ~Message();

    std::ostream& stream() { return *stream_; }

   private:
    std::shared_ptr<std::ostringstream> stream_;
    std::shared_ptr<Logger> logger_;
    const bool visible_;
  };

  Logger();
  virtual ~Logger();

  static void Init(const std::string& file, Level minimum_level);
  static std::shared_ptr<Logger> GetInstance();

  Message AddMessage(Level level);

  std::string file() const { return file_; }

 protected:
  virtual void PostMessage(const std::string& message) = 0;

 private:
  void InitInstance(std::shared_ptr<Logger> self,
      const std::string& file, Level minimum_level);
  std::shared_ptr<Logger> self() const {
    return self_.lock();
  }

  Level minimum_level_;
  std::string file_;
  std::weak_ptr<Logger> self_;

  Logger(const Logger&);
  Logger& operator=(const Logger&);
};

#if defined(OS_WINDOWS)
// MSVC does not support for __func__.
#define __func__ __FUNCTION__
#endif

#define __LOG_STRINGIZE2(v) #v
#define __LOG_STRINGIZE(v) __LOG_STRINGIZE2(v)

#define __LOG_PREFIX(level) \
    std::string(__FILE__  ":" \
    __LOG_STRINGIZE(__LINE__) ":" \
    __LOG_STRINGIZE(level) ":") +\
    __func__ + "(): "

// Use this macro if you want to log something.
// LOG(level) << "Hello!";
// Whele level is: ERROR, WARNING, INFO, DEBUG.
#define LOG(level) \
    (base::Logger::GetInstance()\
        ->AddMessage(base::Logger::LOG_ ## level)\
        .stream() << __LOG_PREFIX(level))

}  // namespace base

// Prevent shared pointer from instance in each file that uses logging.
extern template class std::shared_ptr<base::Logger>;

#endif  // BASE_LOGGING_H_
