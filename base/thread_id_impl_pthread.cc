// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#include "base/thread_id_impl_pthread.h"

#include "base/logging.h"

namespace base {

ThreadIdImplPThread::ThreadIdImplPThread(pthread_t id)
  : id_(id) {}

ThreadIdImplPThread::~ThreadIdImplPThread() {}

bool ThreadIdImplPThread::IsPThread() const {
  return true;
}

const ThreadIdImplPThread* ThreadIdImplPThread::AsPThread() const {
  return this;
}

bool ThreadIdImplPThread::IsEqualTo(const ThreadId::Impl* that) const {
  DCHECK(that->IsPThread());
  return id_ == that->AsPThread()->id_;
}

bool ThreadIdImplPThread::IsLessThan(const ThreadId::Impl* that) const {
  DCHECK(that->IsPThread());
  return id_ < that->AsPThread()->id_;
}

std::unique_ptr<ThreadId::Impl> ThreadIdImplPThread::Copy() const {
  return std::unique_ptr<ThreadId::Impl>(new ThreadIdImplPThread(id_));
}

// static
std::unique_ptr<ThreadId::Impl> ThreadIdImplPThread::GetForCurrentThread() {
  return std::unique_ptr<ThreadId::Impl>(
      new ThreadIdImplPThread(pthread_self()));
}

}  // namespace base

