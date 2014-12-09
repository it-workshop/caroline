// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#include "base/thread_id_pthread.h"

namespace base {

namespace {

const ThreadIdPthread& ToPThread(const ThreadId& id) {
  return static_cast<const ThreadIdPthread&>(id);
}

}  // namespace

ThreadIdPthread::ThreadIdPthread(pthread_t real_id)
  : real_id_(real_id) {}

ThreadIdPthread::~ThreadIdPthread() {}

bool ThreadIdPthread::operator<(const ThreadId& that) const {
  return real_id_ < ToPThread(that).real_id_;
}

bool ThreadIdPthread::operator==(const ThreadId& that) const {
  return real_id_ == ToPThread(that).real_id_;
}

bool ThreadIdPthread::operator!=(const ThreadId& that) const {
  return real_id_ == ToPThread(that).real_id_;
}

std::unique_ptr<ThreadId> ThreadIdPthread::Copy() const {
  return std::unique_ptr<ThreadId>(new ThreadIdPthread(real_id_));
}

// static
std::unique_ptr<ThreadId> ThreadId::GetCurrent() {
  return std::unique_ptr<ThreadId>(new ThreadIdPthread(pthread_self()));
}

}  // namespace base

