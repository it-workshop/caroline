// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// @author Aleksandr Derbenev <alex@technoworks.ru>

#ifndef BASE_REAL_THREAD_H_
#define BASE_REAL_THREAD_H_

#if defined(USE_THREAD_CXX11)
#include <thread>
#elif defined(USE_THREAD_PTHREAD)
#include <pthread.h>
#else
#error USE_THREAD_CXX11 or USE_THREAD_PTHREAD must be defined.
#endif

#include "base/thread.h"

namespace base {

class RealThread : public Thread {
 public:
#if defined(USE_THREAD_CXX11)
  typedef std::thread::id id_t;
#elif defined(USE_THREAD_PTHREAD)
  typedef pthread_t id_t;
#endif

  class Impl {
   public:
    explicit Impl(const RealThread& thread);
    virtual ~Impl() {}

    void Run() const;

    virtual id_t GetId() const = 0;

   private:
    const RealThread& thread_;
  };

  RealThread(
      Type type,
      const std::function<void(const Thread&)>& thread_main,
      std::unique_ptr<MessageLoop>&& message_loop);
  virtual ~RealThread() {}

  static id_t GetCurrentThreadId();

  id_t id() const { return impl_->GetId(); }

 private:
  static std::unique_ptr<Impl> CreateImpl(const RealThread& thread);

  std::function<void(const Thread&)> thread_main_;
  std::unique_ptr<Impl> impl_;
};

}  // namespace base

#endif  // BASE_REAL_THREAD_H_
