// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#include <pthread.h>

#include "base/real_thread_impl_pthread.h"

namespace base {

RealThreadImplPThread::RealThreadImplPThread(const RealThread& thread)
  : RealThread::Impl(thread) {
  pthread_create(&thread_, NULL,
                 (void*(*)(void*))&RealThreadImplPThread::EntryPoint, this);
}

RealThreadImplPThread::~RealThreadImplPThread() {
  pthread_join(thread_, NULL);
}

RealThread::id_t RealThreadImplPThread::GetId() const {
  return thread_;
}

// static
void* RealThreadImplPThread::EntryPoint(const RealThreadImplPThread* self) {
  self->Run();
  return NULL;
}

// static
std::unique_ptr<RealThread::Impl> RealThread::CreateImpl(
    const RealThread& thread) {
  return std::unique_ptr<RealThread::Impl>(new RealThreadImplPThread(thread));
}

// static
RealThread::id_t RealThread::GetCurrentThreadId() {
  return pthread_self();
}

}  // namespace base

