// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#include "base/real_thread_impl_cxx11.h"

#include "base/thread_bundle.h"

namespace base {

RealThreadImplCxx11::RealThreadImplCxx11(const RealThread& thread)
  : RealThread::Impl(thread),
    thread_(EntryPoint, this) {
}

RealThreadImplCxx11::~RealThreadImplCxx11() {
  thread_.join();
}

RealThread::id_t RealThreadImplCxx11::GetId() const {
  return thread_.get_id();
}

// static
void RealThreadImplCxx11::EntryPoint(const RealThreadImplCxx11* self) {
  self->Run();
}

// static
std::unique_ptr<RealThread::Impl> RealThread::CreateImpl(
    const RealThread& thread) {
  return std::unique_ptr<RealThread::Impl>(new RealThreadImplCxx11(thread));
}

// static
RealThread::id_t RealThread::GetCurrentThreadId() {
  return std::this_thread::get_id();
}

// static
void ThreadBundle::Yield() {
  std::this_thread::yield();
}

}  // namespace base
