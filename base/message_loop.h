// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// @author Aleksandr Derbenev <alex@technoworks.ru>

#ifndef BASE_MESSAGE_LOOP_H_
#define BASE_MESSAGE_LOOP_H_

#include <functional>
#include <string>

#include "base/macro.h"

#include "base/threadsafe_queue.h"

namespace base {

class MessageLoop {
 public:
  struct CalledFrom {
    CalledFrom() = default;
    CalledFrom(const std::string& file, unsigned int line)
      : file(file),
        line(line) {}
    std::string file;
    unsigned int line;
  };

  MessageLoop();
  virtual ~MessageLoop();

  void Run();

  void PostTask(const CalledFrom& called_from,
      const std::function<void()>& callback);
  void PostTask(const CalledFrom& called_from,
      std::function<void()>&& callback);

  void Quit();

  static MessageLoop* GetCurrent();

 protected:
  virtual void IdleTask();

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

  DISALLOW_COPY_AND_ASSIGN(MessageLoop);
};

}  // namespace base

#define FROM_HERE base::MessageLoop::CalledFrom(__FILE__, __LINE__)

#endif  // BASE_MESSAGE_LOOP_H_
