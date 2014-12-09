// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#include "base/testing/fake_thread.h"

#include "base/message_loop.h"
#include "base/thread_id_impl_none.h"

namespace base {
namespace testing {

FakeThread::FakeThread(Type type, MessageLoop* message_loop)
  : Thread(type, std::unique_ptr<MessageLoop>(message_loop)),
    id_(std::unique_ptr<ThreadId::Impl>(
        new ThreadIdImplNone(static_cast<int>(type)))) {
  set_message_loop_is_owned(false);
}

FakeThread::~FakeThread() {}

void FakeThread::Start() {}

ThreadId FakeThread::GetId() const {
  return id_;
}

void FakeThread::Yield() const {}

}  // namespace testing
}  // namespace base

