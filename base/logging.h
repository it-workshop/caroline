// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <13alexac@gmail.com>

#ifndef BASE_LOGGING_H_
#define BASE_LOGGING_H_

#include <memory>
#include <sstream>
#include <string>

#include "protocol.pb.h"  // NOLINT

namespace bitdata {

class GlobalMessage;

}

namespace base {

/// Logger serves LOG() macro.
///
/// Each log message can be thrown to the different level.
/// You can use LOG() macro:
///
///     LOG(INFO) << "My message with code: " << some_code << "!";
class Logger {
 public:
  /// Logging levels. LOG_ part will be completed by LOG() macro
  enum Level {
    /// Logging is disabled (default).
    LOG_NONE,
    /// Log only errors.
    LOG_ERROR,
    /// Log errors and warnings.
    LOG_WARNING,
    /// Log info, errors and warnings.
    LOG_INFO,
    /// Log all.
    LOG_DEBUG
  };
  // Constants for level names in the --enable-logging switch.
  /// Error level parameter.
  static const char kLevelError[];
  /// Warning level parameter.
  static const char kLevelWarning[];
  /// Info level parameter.
  static const char kLevelInfo[];
  /// Debug level parameter.
  static const char kLevelDebug[];

  /// Represents single log message.
  class Message {
   public:
    /// Constructs a message with given logger and visibility).
    /// @param[in] logger  Logger to log to.
    /// @param[in] visible Will it log this message.
    Message(std::shared_ptr<Logger> logger, bool visible);
    /// Destructor. Automatically appends "\n" to the end of message and writes
    /// it to the logger if visible is true.
    virtual ~Message();

    /// Stream for collecting a message.
    /// @returns ostream to write log into.
    std::ostream& stream() { return *stream_; }

   private:
    std::shared_ptr<std::ostringstream> stream_;
    std::shared_ptr<Logger> logger_;
    const bool visible_;
  };

  /// Default constructor.
  Logger();
  /// Destructor.
  virtual ~Logger();

  /// Creates logger singleton. Logger will write to the standard output and to
  /// the file if specified.
  /// @param[in] file          Path to the file or socket to log into.
  /// @param[in] minimum_level Minimum level of visible message.
  static void Init(const std::string& file, Level minimum_level);
  /// Logger singleton instance.
  /// @returns singleton instance)
  static std::shared_ptr<Logger> GetInstance();

  /// Creates a message, linked with this logger and level.
  /// @param[in] level Level of the message.
  /// @returns new message.
  Message AddMessage(Level level);

  /// A path to the log file.
  /// @returns path to the log.
  std::string file() const { return file_; }

  void Set_Connection_Data(std::string data) {connection_data_ = data; }

protected:
  /// Internal function that writes a string to the log.
  /// @param[in] message Message to write.
  void PostMessage(const std::string& message);
  virtual void PostMessageImpl(const std::string& message)=0;

 private:
  /// Internal function that initializes logger.
  /// @param self          Shared pointer to this.
  /// @param file          Path to the log file.
  /// @param minimum_level Minimum level of messages.
  void InitInstance(std::shared_ptr<Logger> self,
      const std::string& file, Level minimum_level);
  /// Shared pointer for logger.
  /// @returns shared pointer to this.
  std::shared_ptr<Logger> self() const {
    return self_.lock();
  }

  Level minimum_level_;
  std::string file_;
  std::string connection_data_;
  std::weak_ptr<Logger> self_;

  /// Copy consturctor is disallowed.
  Logger(const Logger&);
  /// Assign operator is disallowed.
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

/// Use this macro if you want to log something.
/// LOG(level) << "Hello!";
/// Whele level is: ERROR, WARNING, INFO, DEBUG.
#define LOG(level) \
    (base::Logger::GetInstance()\
        ->AddMessage(base::Logger::LOG_ ## level)\
        .stream() << __LOG_PREFIX(level))

}  // namespace base

// Prevent shared pointer from instance in each file that uses logging.
extern template class std::shared_ptr<base::Logger>;

#endif  // BASE_LOGGING_H_
