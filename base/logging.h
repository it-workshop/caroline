// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#ifndef BASE_LOGGING_H_
#define BASE_LOGGING_H_

#include <memory>
#include <set>
#include <sstream>
#include <string>

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
  /// Logging-off parameter.
  static const char kLevelNone[];
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

    void Flush();

   private:
    std::shared_ptr<std::ostringstream> stream_;
    std::shared_ptr<Logger> logger_;
    const bool visible_;
    bool posted_;
  };

  // Observer class for log listeners.
  class Observer {
   public:
    // Destructor.
    virtual ~Observer() {}

    /// Called on each message submit.
    /// @param[in] message Log message.
    virtual void Observe(const std::string& message) = 0;
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

  /// Add new observer to the logger.
  /// @param[in] observer Object that will be notified about log messages.
  void AddObserver(Observer* observer);

  /// Check that logger notifies this observer about messages.
  /// @param[in] observer Object to check.
  /// @returns true if observer is notified about messages.
  bool HasObserver(Observer* observer) const;

  /// Remove observer from the listeners list.
  /// @param[in] observer Object to remove from the list.
  void RemoveObserver(Observer* observer);

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
  std::weak_ptr<Logger> self_;

  std::set<Observer*> observers_;

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

class CrashHelper {
 public:
  CrashHelper()
    : message_(Logger::GetInstance()->AddMessage(Logger::LOG_ERROR)) {}

  ~CrashHelper() {
    message_.Flush();
    int* ptr = nullptr;
    *ptr = 0;
  }

  std::ostream& stream() { return message_.stream(); }

 private:
  Logger::Message message_;
};


#define __DCHECK_IMPL ::base::CrashHelper().stream() << __LOG_PREFIX(ERROR)
#define DCHECK(value) if (!value) __DCHECK_IMPL "DCHECK: " #value " "

#define DCHECK_EQ(a, b) if ((a) != (b)) \
    __DCHECK_IMPL "DCHECK_EQ: " #a " != " #b " "
#define DCHECK_NE(a, b) if ((a) == (b)) \
    __DCHECK_IMPL "DCHECK_NE: " #a " == " #b " "
#define DCHECK_GT(a, b) if ((a) <= (b)) \
    __DCHECK_IMPL "DCHECK_GT: " #a " <= " #b " "
#define DCHECK_GE(a, b) if ((a) < (b)) \
    __DCHECK_IMPL "DCHECK_GE: " #a " <= " #b " "
#define DCHECK_LT(a, b) if ((a) >= (b)) \
    __DCHECK_IMPL "DCHECK_LT: " #a " >= " #b " "
#define DCHECK_LE(a, b) if ((a) > (b)) \
    __DCHECK_IMPL "DCHECK_LE: " #a " > " #b " "

#define NOTREACHED() __DCHECK_IMPL "NOTREACHED() "

}  // namespace base

// Prevent shared pointer from instance in each file that uses logging.
extern template class std::shared_ptr<base::Logger>;

#endif  // BASE_LOGGING_H_

