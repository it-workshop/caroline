// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#include "base/lock.h"

namespace base {

Lock::Lock() {
#if defined(USE_LOCK_PTHREAD_MUTEX)
  pthread_mutex_init(&mutex_, NULL);
#elif defined(USE_LOCK_PTHREAD_SPINLOCK)
  pthread_spinlock_init(&spinlock_, PTHREAD_PROCESS_PRIVATE);
#endif
}

Lock::~Lock() {
#if defined(USE_LOCK_PTHREAD_MUTEX)
  pthread_mutex_destroy(&mutex_);
#elif defined(USE_LOCK_PTHREAD_SPINLOCK)
  pthread_spin_destroy(&spinlock_);
#endif
}

void Lock::DoLock() {
#if defined(USE_LOCK_CXX11_ATOMIC)
  while (atomic_.test_and_set()) {}
#elif defined(USE_LOCK_PTHREAD_MUTEX)
  pthread_mutex_lock(&mutex_);
#elif defined(USE_LOCK_PTHREAD_SPINLOCK)
  pthread_spin_lock(&spinlock_);
#endif
}

void Lock::Unlock() {
#if defined(USE_LOCK_CXX11_ATOMIC)
  atomic_.clear();
#elif defined(USE_LOCK_PTHREAD_MUTEX)
  pthread_mutex_unlock(&mutex_);
#elif defined(USE_LOCK_PTHREAD_SPINLOCK)
  pthread_spin_unlock(&spinlock_);
#endif
}

}  // namespace base

