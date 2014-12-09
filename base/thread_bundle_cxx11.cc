// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#include "base/thread_bundle_cxx11.h"

#include "base/thread_cxx11.h"
#include "base/thread_id_impl_cxx11.h"

namespace base {

ThreadBundleCxx11::~ThreadBundleCxx11() {}

std::unique_ptr<Thread> ThreadBundleCxx11::CreateMainThread() const {
  return std::unique_ptr<Thread>(new ThreadMainCxx11());
}

std::unique_ptr<Thread> ThreadBundleCxx11::CreateIoThread() const {
  return std::unique_ptr<Thread>(new ThreadCxx11(Thread::kIoThread));
}

std::unique_ptr<Thread> ThreadBundleCxx11::CreateWorkerThread() const {
  return std::unique_ptr<Thread>(new ThreadCxx11(Thread::kWorkerThread));
}

ThreadId ThreadBundleCxx11::GetCurrentThreadId() const {
  return ThreadId(ThreadIdImplCxx11::GetForCurrentThread());
}

}  // namespace base

