// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// @author Aleksandr Derbenev <alex@technoworks.ru>

#include "base/message_loop.h"

#include <cassert>

#include "base/thread_bundle.h"

namespace base {

MessageLoop::MessageLoop()
  : state_(kNotStartedState) {}

MessageLoop::~MessageLoop() {
  assert(state_ == kNotStartedState || state_ == kStoppedState);
}

void  MessageLoop::Run() {
  if (state_ != kNotStartedState) {
    return;
  }

  state_ = kRunningState;

  while (state_ == kRunningState) {
    Task task;
    while (state_ == kRunningState &&
        task_queue_.Dequeue(&task))
      task.callback();

    IdleTask();

    ThreadBundle::Yield();
  }

  state_ = kStoppedState;
}

void MessageLoop::PostTask(
    const CalledFrom& called_from,
    const std::function<void()>& callback) {
  task_queue_.Enqueue(Task(callback, called_from));
}

void MessageLoop::PostTask(
    const CalledFrom& called_from,
    std::function<void()>&& callback) {
  task_queue_.Enqueue(Task(std::move(callback), called_from));
}

void MessageLoop::Quit() {
  switch (state_) {
    case kNotStartedState:
      state_ = kStoppedState;
      break;
    case kRunningState:
      state_ = kStoppingState;
      break;
    default:
      break;
  }
}

void MessageLoop::IdleTask() {
}

// static
MessageLoop* MessageLoop::GetCurrent() {
  return ThreadBundle::GetInstance()->GetCurrentThread()->message_loop();
}

}  // namespace base
