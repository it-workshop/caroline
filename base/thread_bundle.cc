// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// @author Aleksandr Derbenev <alex@technoworks.ru>

#include "base/thread_bundle.h"

#include <string>

#include "base/command_line.h"
#include "base/logging.h"
#include "base/main_thread.h"
#include "base/message_loop.h"
#include "base/testing/thread_bundle.h"
#include "base/switches.h"

#if defined(USE_THREAD_CXX11)
#include "base/thread_bundle_cxx11.h"
#endif
#if defined(USE_THREAD_PTHREAD)
#include "base/thread_bundle_pthread.h"
#endif

INSTANCE_SINGLETON(base::ThreadBundle);

namespace base {

namespace {

#if defined(USE_THREAD_CXX11)
const char kThreadingCxx11[] = "c++11";
#endif
#if defined(USE_THREAD_PTHREAD)
const char kThreadingPThread[] = "pthread";
#endif
const char kThreadingNone[] = "none";
#if defined(USE_THREAD_PTRHEAD)
const char* kThreadingDefault = kThreadingPThread;
#elif defined(USE_THREAD_CXX11)
const char* kThreadingDefault = kThreadingCxx11;
#else
const char* kThreadingDefault = kThreadingNone;
#endif

}  // namespace

ThreadBundle::ThreadBundle() {}

ThreadBundle::~ThreadBundle() {
  PostQuitTasks();
}

// static
ThreadBundle* ThreadBundle::Create() {
  std::string threading_type(kThreadingDefault);
  std::shared_ptr<CommandLine> command_line =
      CommandLine::GetForCurrentProcess();
  if (command_line) {
    if (command_line->HasSwitch(switches::kSingleThread)) {
      threading_type = kThreadingNone;
    } else if (command_line->HasSwitch(switches::kThreading)) {
      threading_type = command_line->GetSwitchData(switches::kThreading);
    }
  }

  while (true) {
#if defined(USE_THREAD_CXX11)
    if (threading_type == kThreadingCxx11)
      return new ThreadBundleCxx11();
#endif
#if defined(USE_THREAD_PTHREAD)
    if (threading_type == kThreadingPThread)
      return new ThreadBundlePThread();
#endif
    if (threading_type == kThreadingNone)
      return new testing::ThreadBundle();
    threading_type = kThreadingDefault;
  }
}

void ThreadBundle::Start() {
  main_thread_ = CreateMainThread();
  io_thread_ = CreateIoThread();
  worker_thread_ = CreateWorkerThread();

  thread_by_id_[main_thread_->GetId()] = main_thread_.get();
  thread_by_id_[io_thread_->GetId()] = io_thread_.get();
  thread_by_id_[worker_thread_->GetId()] = worker_thread_.get();

  main_thread_->Start();
  io_thread_->Start();
  worker_thread_->Start();
}

const Thread* ThreadBundle::Get(Thread::Type type) const {
  switch (type) {
    case Thread::kMainThread:
      return main_thread_.get();
    case Thread::kIoThread:
      return io_thread_.get();
    case Thread::kWorkerThread:
      return worker_thread_.get();
    default:
      NOTREACHED() << "Unknown thread type.";
  }
  return main_thread_.get();
}

bool ThreadBundle::IsOnThread(Thread::Type type) const {
  return type == GetCurrentThread()->type();
}

const Thread* ThreadBundle::GetCurrentThread() const {
  const ThreadId& id = GetCurrentThreadId();
  std::map<ThreadId, Thread*>::const_iterator it = thread_by_id_.find(id);
  if (it != thread_by_id_.end())
    return it->second;
  NOTREACHED() << "Unknown thread.";
  return main_thread_.get();
}

void ThreadBundle::PostQuitTasks() const {
  auto task = ([] (MessageLoop* loop) { loop->Quit(); });
  io_thread_->message_loop()->PostTask(FROM_HERE,
      std::bind(task, io_thread_->message_loop()));
  worker_thread_->message_loop()->PostTask(FROM_HERE,
      std::bind(task, worker_thread_->message_loop()));
}

}  // namespace base

