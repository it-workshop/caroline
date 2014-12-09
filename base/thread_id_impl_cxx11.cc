// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#include "base/thread_id_impl_cxx11.h"

#include "base/logging.h"

namespace base {

ThreadIdImplCxx11::ThreadIdImplCxx11(std::thread::id id)
  : id_(id) {}

ThreadIdImplCxx11::~ThreadIdImplCxx11() {}

bool ThreadIdImplCxx11::IsCxx11() const {
  return true;
}

const ThreadIdImplCxx11* ThreadIdImplCxx11::AsCxx11() const {
  return this;
}

bool ThreadIdImplCxx11::IsEqualTo(const ThreadId::Impl* that) const {
  DCHECK(that->IsCxx11());
  return id_ == that->AsCxx11()->id_;
}

bool ThreadIdImplCxx11::IsLessThan(const ThreadId::Impl* that) const {
  DCHECK(that->IsCxx11());
  return id_ < that->AsCxx11()->id_;
}

std::unique_ptr<ThreadId::Impl> ThreadIdImplCxx11::Copy() const {
  return std::unique_ptr<ThreadId::Impl>(new ThreadIdImplCxx11(id_));
}

// static
std::unique_ptr<ThreadId::Impl> ThreadIdImplCxx11::GetForCurrentThread() {
  return std::unique_ptr<ThreadId::Impl>(
      new ThreadIdImplCxx11(std::this_thread::get_id()));
}

}  // namespace base

