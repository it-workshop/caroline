// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// @author Aleksandr Derbenev <alex@technoworks.ru>

#include "base/thread_bundle.h"

#include <cassert>

#include "base/main_thread.h"
#include "base/message_loop.h"
#include "base/real_thread.h"

INSTANCE_SINGLETON(base::ThreadBundle);

namespace base {

ThreadBundle::ThreadBundle() {
  auto thread_main = ([] (const Thread& thread) {
        thread.message_loop()->Run();
      });

  main_thread_.reset(new MainThread(
      std::unique_ptr<MessageLoop>(new MessageLoop())));
  io_thread_.reset(new RealThread(
      Thread::kIoThread,
      thread_main,
      std::unique_ptr<MessageLoop>(new MessageLoop())));
  worker_thread_.reset(new RealThread(
      Thread::kWorkerThread,
      thread_main,
      std::unique_ptr<MessageLoop>(new MessageLoop())));
}

ThreadBundle::~ThreadBundle() {}

const Thread* ThreadBundle::Get(Thread::Type type) const {
  switch (type) {
    case Thread::kMainThread:
      return main_thread_.get();
    case Thread::kIoThread:
      return io_thread_.get();
    case Thread::kWorkerThread:
      return worker_thread_.get();
    default:
      assert(false);
  }
  return main_thread_.get();
}

bool ThreadBundle::IsOnThread(Thread::Type type) const {
  return Get(type) == GetCurrentThread();
}

const Thread* ThreadBundle::GetCurrentThread() const {
  const RealThread::id_t thread_id = RealThread::GetCurrentThreadId();
  if (io_thread_->id() == thread_id)
    return io_thread_.get();

  if (worker_thread_->id() == thread_id)
    return worker_thread_.get();

  return main_thread_.get();
}

}  // namespace base
