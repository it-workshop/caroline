// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// @author Aleksandr Derbenev <alex@technoworks.ru>

#ifndef BASE_MESSAGE_LOOP_IMPL_H_
#define BASE_MESSAGE_LOOP_IMPL_H_

#include "base/message_loop.h"

#include "base/threadsafe_queue.h"

namespace base {

class MessageLoopImpl : public MessageLoop {
 public:
  MessageLoopImpl();
  virtual ~MessageLoopImpl();

  virtual void Run() override;

  virtual void PostTask(const CalledFrom& called_from,
                        const std::function<void()>& callback) override;
  virtual void PostTask(const CalledFrom& called_from,
                        std::function<void()>&& callback) override;

  virtual void Quit() override;

 private:
  enum State {
    kNotStartedState,
    kRunningState,
    kStoppingState,
    kStoppedState
  };

  State state_;

  struct Task {
    Task() = default;
    Task(const std::function<void()>& callback,
        const CalledFrom& called_from)
      : callback(callback),
        called_from(called_from) {}
    Task(std::function<void()>&& callback,
        const CalledFrom& called_from)
      : callback(callback),
        called_from(called_from) {}

    std::function<void()> callback;
    CalledFrom called_from;
  };

  ThreadSafeQueue<Task> task_queue_;
};

}  // namespace base

#endif  // BASE_MESSAGE_LOOP_IMPL_H_

