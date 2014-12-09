// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#include "base/thread_id_impl_none.h"

#include "base/logging.h"

namespace base {

ThreadIdImplNone::ThreadIdImplNone(int id)
  : id_(id) {}

ThreadIdImplNone::~ThreadIdImplNone() {}

bool ThreadIdImplNone::IsEqualTo(const ThreadId::Impl* that) const {
  DCHECK(that->IsNone());
  return id_ == that->AsNone()->id_;
}

bool ThreadIdImplNone::IsLessThan(const ThreadId::Impl* that) const {
  DCHECK(that->IsNone());
  return id_ < that->AsNone()->id_;
}

std::unique_ptr<ThreadId::Impl> ThreadIdImplNone::Copy() const {
  return std::unique_ptr<ThreadId::Impl>(new ThreadIdImplNone(id_));
}

bool ThreadIdImplNone::IsNone() const {
  return true;
}

const ThreadIdImplNone* ThreadIdImplNone::AsNone() const {
  return this;
}

}  // namespace base

