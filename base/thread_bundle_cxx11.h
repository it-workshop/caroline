// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#ifndef BASE_THREAD_BUNDLE_CXX11_H_
#define BASE_THREAD_BUNDLE_CXX11_H_

#include "base/thread_bundle.h"

namespace base {

class ThreadBundleCxx11 : public ThreadBundle {
 public:
  virtual ~ThreadBundleCxx11();

 protected:
  virtual std::unique_ptr<Thread> CreateMainThread() const override;
  virtual std::unique_ptr<Thread> CreateIoThread() const override;
  virtual std::unique_ptr<Thread> CreateWorkerThread() const override;
  virtual ThreadId GetCurrentThreadId() const override;
};

}  // namespace base

#endif  // BASE_THREAD_BUNDLE_CXX11_H_

