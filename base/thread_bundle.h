// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// @author Aleksandr Derbenev <alex@technoworks.ru>

#ifndef BASE_THREAD_BUNDLE_H_
#define BASE_THREAD_BUNDLE_H_

#include "base/singleton.h"
#include "base/thread.h"

namespace base {

class RealThread;

class ThreadBundle : public Singleton<ThreadBundle> {
 public:
  ThreadBundle();
  virtual ~ThreadBundle();

  const Thread* Get(Thread::Type type) const;

  const Thread* GetCurrentThread() const;

  bool IsOnThread(Thread::Type type) const;
  bool IsOnMainThread() const { return IsOnThread(Thread::kMainThread); }
  bool IsOnIOThread() const { return IsOnThread(Thread::kIoThread); }

 private:
  std::unique_ptr<Thread> main_thread_;
  std::unique_ptr<RealThread> io_thread_;
  std::unique_ptr<RealThread> worker_thread_;
};

}  // namespace base

#endif  // BASE_THREAD_BUNDLE_H_
