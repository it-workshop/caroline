// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#ifndef BASE_THREAD_H_
#define BASE_THREAD_H_

#include <functional>
#include <memory>

#include "base/thread_id.h"

namespace base {

class MessageLoop;

/// Interface for thread. Must be implemented in the children classes.
class Thread {
 public:
  /// Type of this thread.
  enum Type {
    kMainThread,
    kIoThread,
    kWorkerThread,
    kThreadTypesCount
  };

  /// Constructor.
  /// @param[in] type Type of this thread.
  /// @param[in] message_lopp Message loop to run on this thread.
  Thread(Type type, std::unique_ptr<MessageLoop>&& message_loop);
  /// Dsetructor.
  virtual ~Thread();


  /// Get type of thread.
  /// @returns thread type.
  Type type() const { return type_; }

  /// Get message loop.
  /// @returns message loop of this thread.
  MessageLoop* message_loop() const { return message_loop_.get(); }

  /// Start execution.
  virtual void Start() = 0;

  /// Get this thread id.
  /// @returns thread if for this thread.
  virtual ThreadId GetId() const = 0;

  /// Yield current thread.
  virtual void Yield() const = 0;

 protected:
  /// @returns true if this thread owns message_loop, false if message loop
  /// will not be deleted on destructor. True by default.
  bool message_loop_is_owned() const { return message_loop_is_owned_; }
  /// Set message loop is owned by this thread.
  /// @param[in] owned true if message loop must be deleted in destructor
  /// (true by default).
  void set_message_loop_is_owned(bool owned) {
    message_loop_is_owned_ = owned;
  }

 private:
  Type type_;
  bool message_loop_is_owned_;
  std::unique_ptr<MessageLoop> message_loop_;
};

}  // namespace base

#endif  // BASE_THREAD_H_
