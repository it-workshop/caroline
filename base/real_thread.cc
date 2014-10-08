// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// @author Aleksandr Derbenev <alex@technoworks.ru>

#include "base/real_thread.h"

#include <cassert>

namespace base {

RealThread::Impl::Impl(const RealThread& thread)
  : thread_(thread) {}

void RealThread::Impl::Run() const {
  thread_.thread_main_(thread_);
}

RealThread::RealThread(Type type,
                       const std::function<void(const Thread&)>& thread_main,
                       std::unique_ptr<MessageLoop>&& message_loop)
  : Thread(type, std::move(message_loop)),
    thread_main_(thread_main) {
  assert(type != kMainThread);
  assert(type != kThreadTypesCount);
  impl_ = CreateImpl(*this);
}

}  // namespace base
