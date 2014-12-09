// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#ifndef BASE_THREAD_PTHREAD_H_
#define BASE_THREAD_PTHREAD_H_

#include <pthread.h>

#include "base/thread.h"

namespace base {

class ThreadPThread : public Thread {
 public:
  explicit ThreadPThread(Thread::Type type);
  virtual ~ThreadPThread();

  virtual void Start() override;
  virtual ThreadId GetId() const override;
  virtual void Yield() const override;

 private:
  static void* EntryPoint(const ThreadPThread* thread);
  void Run() const;

  pthread_t thread_;
};

class ThreadMainPThread : public Thread {
 public:
  ThreadMainPThread();
  virtual ~ThreadMainPThread();

  virtual void Start() override;
  virtual ThreadId GetId() const override;
  virtual void Yield() const override;

 private:
  ThreadId id_;
};

}  // namespace base

#endif  // BASE_THREAD_PTHREAD_H_

