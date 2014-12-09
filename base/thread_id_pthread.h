// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#ifndef BASE_THREAD_ID_PTHREAD_H_
#define BASE_THREAD_ID_PTHREAD_H_

#include <pthread.h>

#include "base/thread_id.h"

namespace base {

/// Thread id implementation for pthreads.
class ThreadIdPthread : public ThreadId {
 public:
  /// Constructor.
  /// @param[in] real_id Real pthread handle.
  explicit ThreadIdPthread(pthread_t real_id);
  /// Destructor.
  virtual ~ThreadIdPthread();

  // Overriden from base::ThreadId:
  bool operator<(const ThreadId& that) const override;
  bool operator==(const ThreadId& that) const override;
  bool operator!=(const ThreadId& that) const override;
  std::unique_ptr<ThreadId> Copy() const override;

 private:
  pthread_t real_id_;
};

}  // namespace base

#endif  // BASE_THREAD_ID_PTHREAD_H_

