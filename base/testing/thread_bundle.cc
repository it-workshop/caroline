// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#include "base/testing/thread_bundle.h"

#include "base/testing/fake_thread.h"
#include "base/testing/message_loop.h"

namespace base {
namespace testing {

namespace {

std::unique_ptr<Thread> CreateFakeThread(Thread::Type type,
                                         MessageLoop* message_loop) {
  return std::unique_ptr<Thread>(
      new FakeThread(type, message_loop->GetForThread(static_cast<int>(type))));
}

}  // namespace

ThreadBundle::ThreadBundle()
  : message_loop_(new MessageLoop()) {}

ThreadBundle::~ThreadBundle() {}

ThreadId ThreadBundle::GetCurrentThreadId() const {
  return message_loop_->current_thread();
}

std::unique_ptr<Thread> ThreadBundle::CreateMainThread() const {
  return CreateFakeThread(Thread::kMainThread, message_loop_.get());
}

std::unique_ptr<Thread> ThreadBundle::CreateIoThread() const {
  return CreateFakeThread(Thread::kIoThread, message_loop_.get());
}

std::unique_ptr<Thread> ThreadBundle::CreateWorkerThread() const {
  return CreateFakeThread(Thread::kWorkerThread, message_loop_.get());
}

}  // namespace testing
}  // namespace base

