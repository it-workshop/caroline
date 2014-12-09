// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#ifndef BASE_TESTING_THREAD_BUNDLE_H_
#define BASE_TESTING_THREAD_BUNDLE_H_

#include "base/thread_bundle.h"

namespace base {
namespace testing {

class MessageLoop;

/// Single thread implementation of thread bundle.
class ThreadBundle : public ::base::ThreadBundle {
 public:
  ThreadBundle();
  virtual ~ThreadBundle();

 protected:
  virtual ThreadId GetCurrentThreadId() const override;
  virtual std::unique_ptr<Thread> CreateMainThread() const override;
  virtual std::unique_ptr<Thread> CreateIoThread() const override;
  virtual std::unique_ptr<Thread> CreateWorkerThread() const override;

 private:
  std::unique_ptr<MessageLoop> message_loop_;
};

}  // namespace testing
}  // namespace base

#endif  // BASE_TESTING_THREAD_BUNDLE_H_

