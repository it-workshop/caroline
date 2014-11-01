// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#ifndef BASE_REAL_THREAD_IMPL_PTHREAD_H_
#define BASE_REAL_THREAD_IMPL_PTHREAD_H_

#include <pthread.h>

#include "base/real_thread.h"

namespace base {

class RealThreadImplPThread : public RealThread::Impl {
 public:
  explicit RealThreadImplPThread(const RealThread& thread);
  virtual ~RealThreadImplPThread();

  virtual RealThread::id_t GetId() const override;

  static void* EntryPoint(const RealThreadImplPThread* self);

 private:
  pthread_t thread_;
};

}  // namespace base

#endif  // BASE_REAL_THREAD_IMPL_PTHREAD_H_

