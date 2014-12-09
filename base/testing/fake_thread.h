// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#ifndef BASE_TESTING_FAKE_THREAD_H_
#define BASE_TESTING_FAKE_THREAD_H_

#include "base/thread.h"

namespace base {
namespace testing {

class FakeThread : public Thread {
 public:
  FakeThread(Type type, ::base::MessageLoop* message_loop);
  virtual ~FakeThread();

  virtual void Start() override;
  virtual ThreadId GetId() const override;
  virtual void Yield() const override;

 private:
  ThreadId id_;
};

}  // namespace testing
}  // namespace base

#endif  // BASE_TESTING_FAKE_THREAD_H_

