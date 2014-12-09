// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#include "base/thread_pthread.h"

#include <sched.h>

#include "base/message_loop_impl.h"
#include "base/thread_id_impl_pthread.h"

namespace base {

ThreadPThread::ThreadPThread(Thread::Type type)
  : Thread(type, std::unique_ptr<MessageLoop>(new MessageLoopImpl())) {
  pthread_create(
      &thread_,
      nullptr,
      (void*(*)(void*))&ThreadPThread::EntryPoint, this);
}

ThreadPThread::~ThreadPThread() {
  pthread_join(thread_, nullptr);
}

void ThreadPThread::Start() {}

ThreadId ThreadPThread::GetId() const {
  return ThreadId(std::unique_ptr<ThreadId::Impl>(
      new ThreadIdImplPThread(thread_)));
}

void ThreadPThread::Yield() const {
  sched_yield();
}

// static
void* ThreadPThread::EntryPoint(const ThreadPThread* thread) {
  thread->Run();
  return nullptr;
}

void ThreadPThread::Run() const {
  message_loop()->Run();
}

ThreadMainPThread::ThreadMainPThread()
  : Thread(Thread::kMainThread,
           std::unique_ptr<MessageLoop>(new MessageLoopImpl())),
    id_(ThreadIdImplPThread::GetForCurrentThread()) {}

ThreadMainPThread::~ThreadMainPThread() {}

void ThreadMainPThread::Start() {}

ThreadId ThreadMainPThread::GetId() const {
  return id_;
}

void ThreadMainPThread::Yield() const {
  sched_yield();
}

}  // namespace base

