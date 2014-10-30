// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#ifndef BASE_LOCK_H_
#define BASE_LOCK_H_

#if defined(USE_LOCK_CXX11_ATOMIC)
#include <atomic>
#elif defined(USE_LOCK_PTHREAD_MUTEX) || defined(USE_LOCK_PTHREAD_SPINLOCK)
#include <pthread.h>
#else
#error One of USE_LOCK_CXX11_ATOMIC, USE_LOCK_PTHREAD_MUTEX or \
    USE_LOCK_PTHREAD_SPINLOCK must be defined.
#endif

#include "base/macro.h"

namespace base {

class Lock {
 public:
  Lock();
  ~Lock();

  void DoLock();
  void Unlock();

 private:
#if defined(USE_LOCK_CXX11_ATOMIC)
  std::atomic_flag atomic_;
#elif defined(USE_LOCK_PTHREAD_MUTEX)
  pthread_mutex_t mutex_;
#elif defined(USE_LOCK_PTHREAD_SPINLOCK)
  pthread_spinlock_t spinlock_;
#endif

  DISALLOW_COPY_AND_ASSIGN(Lock);
};

class ScopedLock {
 public:
  explicit ScopedLock(Lock& lock)  // NOLINT(runtime/references)
    : lock_(lock) {
    lock_.DoLock();
  }

  ~ScopedLock() {
    lock_.Unlock();
  }

 private:
  Lock& lock_;

  DISALLOW_COPY_AND_ASSIGN(ScopedLock);
};

}  // namespace base

#endif  // BASE_LOCK_H_

