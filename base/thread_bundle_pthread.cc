// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#include "base/thread_bundle_pthread.h"

#include "base/thread_id_impl_pthread.h"
#include "base/thread_pthread.h"

namespace base {

ThreadBundlePThread::~ThreadBundlePThread() {}

std::unique_ptr<Thread> ThreadBundlePThread::CreateMainThread() const {
  return std::unique_ptr<Thread>(new ThreadMainPThread());
}

std::unique_ptr<Thread> ThreadBundlePThread::CreateIoThread() const {
  return std::unique_ptr<Thread>(new ThreadPThread(Thread::kIoThread));
}

std::unique_ptr<Thread> ThreadBundlePThread::CreateWorkerThread() const {
  return std::unique_ptr<Thread>(new ThreadPThread(Thread::kWorkerThread));
}

ThreadId ThreadBundlePThread::GetCurrentThreadId() const {
  return ThreadId(ThreadIdImplPThread::GetForCurrentThread());
}

}  // namespace base

