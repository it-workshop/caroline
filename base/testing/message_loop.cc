// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// @author Aleksandr Derbenev <alex@technoworks.ru>

#include "base/testing/message_loop.h"

#include "base/logging.h"
#include "base/thread_id_impl_none.h"

namespace base {
namespace testing {

namespace {

class Proxy : public ::base::MessageLoop {
 public:
  Proxy(::base::testing::MessageLoop* message_loop, int thread)
    : message_loop_(message_loop),
      thread_(thread) {}

  virtual void Run() override {
    message_loop_->Run();
  }

  virtual void PostTask(const CalledFrom& called_from,
                        const std::function<void()>& callback) override {
    message_loop_->PostTaskOnThread(called_from, callback, thread_);
  }

  virtual void PostTask(const CalledFrom& called_from,
                        std::function<void()>&& callback) override {
    message_loop_->PostTaskOnThread(called_from, callback, thread_);
  }

  virtual void Quit() override {
    message_loop_->Quit();
  }

 private:
  ::base::testing::MessageLoop* message_loop_;
  int thread_;
};

}  // namespace

struct MessageLoop::Task {
  CalledFrom called_from;
  std::function<void()> callback;
  int thread;

  Task(CalledFrom called_from,
       const std::function<void()>& callback,
       int thread)
    : called_from(called_from),
      callback(callback),
      thread(thread) {}

  Task(CalledFrom called_from,
       std::function<void()>&& callback,
       int thread)
    : called_from(called_from),
      callback(std::move(callback)),
      thread(thread) {}
};

MessageLoop::MessageLoop()
  : state_(kNotStartedState),
    current_thread_(0) {}

MessageLoop::~MessageLoop() {
  DCHECK_NE(kRunningState, state_);
  DCHECK_NE(kFinishingState, state_);
}

void MessageLoop::Run() {
  if (kNotStartedState != state_)
    return;

  state_ = kRunningState;
  while (kRunningState == state_) {
    DCHECK(!task_queue_.empty());
    current_thread_ = task_queue_.front()->thread;
    task_queue_.front()->callback();
    task_queue_.pop();
  }

  state_ = kFinishedState;
}

void MessageLoop::PostTask(const CalledFrom& called_from,
    const std::function<void()>& callback) {
  PostTaskOnThread(called_from, callback, current_thread_);
}

void MessageLoop::PostTask(const CalledFrom& called_from,
    std::function<void()>&& callback) {
  PostTaskOnThread(called_from, std::move(callback), current_thread_);
}

void MessageLoop::Quit() {
  if (kNotStartedState == state_)
    state_ = kFinishedState;

  if (kRunningState == state_)
    state_ = kFinishingState;
}

::base::MessageLoop* MessageLoop::GetForThread(int thread) {
  return new Proxy(this, thread);
}

void MessageLoop::PostTaskOnThread(const CalledFrom& called_from,
                                   const std::function<void()>& callback,
                                   int thread) {
  task_queue_.push(std::unique_ptr<Task>(
      new Task(called_from, callback, thread)));
}

void MessageLoop::PostTaskOnThread(const CalledFrom& called_from,
                                   std::function<void()>&& callback,
                                   int thread) {
  task_queue_.push(std::unique_ptr<Task>(
      new Task(called_from, std::move(callback), thread)));
}

ThreadId MessageLoop::current_thread() const {
  return ThreadId(std::unique_ptr<ThreadId::Impl>(
      new ThreadIdImplNone(current_thread_)));
}

}  // namespace testing
}  // namespace base

