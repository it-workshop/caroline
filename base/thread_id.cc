// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#include "base/thread_id.h"

namespace base {

ThreadId::ThreadId(std::unique_ptr<Impl>&& impl)
  : impl_(impl.release()) {}

ThreadId::ThreadId(const ThreadId& that)
  : impl_(std::move(that.impl_->Copy())) {}

ThreadId::ThreadId(ThreadId&& that)
  : impl_(std::move(that.impl_)) {}

bool ThreadId::operator<(const ThreadId& that) const {
  return impl_->IsLessThan(that.impl_.get());
}

bool ThreadId::operator==(const ThreadId& that) const {
  return impl_->IsEqualTo(that.impl_.get());
}

bool ThreadId::operator!=(const ThreadId& that) const {
  return !impl_->IsEqualTo(that.impl_.get());
}

ThreadId::Impl::Impl() {}

ThreadId::Impl::~Impl() {}

#if defined(USE_THREAD_CXX11)
bool ThreadId::Impl::IsCxx11() const {
  return false;
}

const ThreadIdImplCxx11* ThreadId::Impl::AsCxx11() const {
  return nullptr;
}
#endif

#if defined(USE_THREAD_PTHREAD)
bool ThreadId::Impl::IsPThread() const {
  return false;
}

const ThreadIdImplPThread* ThreadId::Impl::AsPThread() const {
  return nullptr;
}
#endif

bool ThreadId::Impl::IsNone() const {
  return false;
}

const ThreadIdImplNone* ThreadId::Impl::AsNone() const {
  return nullptr;
}

}  // namespace base

