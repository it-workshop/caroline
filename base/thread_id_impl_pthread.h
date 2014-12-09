// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#ifndef BASE_THREAD_ID_IMPL_PTHREAD_H_
#define BASE_THREAD_ID_IMPL_PTHREAD_H_

#include <pthread.h>

#include "base/thread_id.h"

namespace base {

class ThreadIdImplPThread : public ThreadId::Impl {
 public:
  explicit ThreadIdImplPThread(pthread_t id);
  virtual ~ThreadIdImplPThread();

  virtual bool IsPThread() const override;
  virtual const ThreadIdImplPThread* AsPThread() const override;

  virtual bool IsEqualTo(const ThreadId::Impl* that) const override;
  virtual bool IsLessThan(const ThreadId::Impl* that) const override;
  virtual std::unique_ptr<ThreadId::Impl> Copy() const override;

  static std::unique_ptr<ThreadId::Impl> GetForCurrentThread();

 private:
  pthread_t id_;
};

}  // namespace base

#endif  // BASE_THREAD_ID_IMPL_PTHREAD_H_

