// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#ifndef BASE_THREAD_CXX11_H_
#define BASE_THREAD_CXX11_H_

#include <thread>

#include "base/thread.h"

namespace base {

class ThreadCxx11 : public Thread {
 public:
  explicit ThreadCxx11(Thread::Type type);
  virtual ~ThreadCxx11();

  virtual void Start() override;
  virtual ThreadId GetId() const override;
  virtual void Yield() const override;

 private:
  void Run();

  std::thread thread_;
};

class ThreadMainCxx11 : public Thread {
 public:
  ThreadMainCxx11();
  virtual ~ThreadMainCxx11();

  virtual void Start() override;
  virtual ThreadId GetId() const override;
  virtual void Yield() const override;

 private:
  ThreadId id_;
};

}  // namespace base

#endif  // BASE_THREAD_CXX11_H_

