// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#ifndef BASE_TESTING_MESSAGE_LOOP_H_
#define BASE_TESTING_MESSAGE_LOOP_H_

#include <memory>
#include <queue>

#include "base/message_loop.h"
#include "base/thread_id.h"

namespace base {
namespace testing {

class MessageLoop : public ::base::MessageLoop {
 public:
  MessageLoop();
  virtual ~MessageLoop();

  virtual void Run() override;

  virtual void PostTask(const CalledFrom& called_from,
                        const std::function<void()>& callback) override;
  virtual void PostTask(const CalledFrom& called_from,
                         std::function<void()>&& callback) override;
  virtual void Quit() override;

  ::base::MessageLoop* GetForThread(int thread);

  void PostTaskOnThread(const CalledFrom& called_from,
                        const std::function<void()>& callback,
                        int thread);
  void PostTaskOnThread(const CalledFrom& called_from,
                        std::function<void()>&& callback,
                        int thread);

  ThreadId current_thread() const;

 private:
  struct Task;
  enum State {
    kNotStartedState,
    kRunningState,
    kFinishingState,
    kFinishedState
  };

  State state_;
  std::queue<std::unique_ptr<Task>> task_queue_;
  int current_thread_;
};

}  // namespace testing
}  // namespace base

#endif  // BASE_TESTING_MESSAGE_LOOP_H_

