// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#ifndef BASE_THREAD_H_
#define BASE_THREAD_H_

#include <functional>
#include <memory>

namespace base {

class MessageLoop;

class Thread {
 public:
  enum Type {
    kMainThread,
    kIoThread,
    kWorkerThread,
    kThreadTypesCount
  };

  Thread(Type type, std::unique_ptr<MessageLoop>&& message_loop);
  virtual ~Thread();

  Type type() const { return type_; }

  MessageLoop* message_loop() const { return message_loop_.get(); }

 private:
  Type type_;
  std::unique_ptr<MessageLoop> message_loop_;
};

}  // namespace base

#endif  // BASE_THREAD_H_
