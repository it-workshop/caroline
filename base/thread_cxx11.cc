// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#include "base/thread_cxx11.h"

#include "base/message_loop.h"
#include "base/thread_id_impl_cxx11.h"

namespace base {

ThreadCxx11::ThreadCxx11(Thread::Type type)
  : Thread(type, std::unique_ptr<MessageLoop>(new MessageLoop())),
    thread_([this] () { this->Run(); }) {
}

ThreadCxx11::~ThreadCxx11() {
  thread_.join();
}

void ThreadCxx11::Start() {
}

ThreadId ThreadCxx11::GetId() const {
  return ThreadId(std::unique_ptr<ThreadId::Impl>(
      new ThreadIdImplCxx11(thread_.get_id())));
}

void ThreadCxx11::Yield() const {
  std::this_thread::yield();
}

void ThreadCxx11::Run() {
  message_loop()->Run();
}

ThreadMainCxx11::ThreadMainCxx11()
  : Thread(Thread::kMainThread,
           std::unique_ptr<MessageLoop>(new MessageLoop())),
    id_(ThreadIdImplCxx11::GetForCurrentThread()) {}

ThreadMainCxx11::~ThreadMainCxx11() {}

void ThreadMainCxx11::Start() {}

ThreadId ThreadMainCxx11::GetId() const {
  return id_;
}

void ThreadMainCxx11::Yield() const {
  std::this_thread::yield();
}

}  // namespace base

