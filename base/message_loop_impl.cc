// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// @author Aleksandr Derbenev <alex@technoworks.ru>

#include "base/message_loop_impl.h"

#include "base/logging.h"
#include "base/thread_bundle.h"

namespace base {

MessageLoopImpl::MessageLoopImpl()
  : state_(kNotStartedState) {}

MessageLoopImpl::~MessageLoopImpl() {
  DCHECK_NE(kNotStartedState, state_);
  DCHECK_NE(kStoppedState, state_);
}

void  MessageLoopImpl::Run() {
  DCHECK_EQ(state_, kNotStartedState);

  state_ = kRunningState;
  while (state_ == kRunningState) {
    Task task;
    while (state_ == kRunningState &&
        task_queue_.Dequeue(&task))
      task.callback();

    ThreadBundle::GetInstance()->GetCurrentThread()->Yield();
  }

  state_ = kStoppedState;
}

void MessageLoopImpl::PostTask(
    const CalledFrom& called_from,
    const std::function<void()>& callback) {
  task_queue_.Enqueue(Task(callback, called_from));
}

void MessageLoopImpl::PostTask(
    const CalledFrom& called_from,
    std::function<void()>&& callback) {
  task_queue_.Enqueue(Task(std::move(callback), called_from));
}

void MessageLoopImpl::Quit() {
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

}  // namespace base

