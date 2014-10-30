// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#include "base/message_loop.h"
#include "base/real_thread.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::StrictMock;
using ::testing::ExpectationSet;
using ::testing::Exactly;

namespace base {

namespace {

class ThreadMock {
 public:
  MOCK_METHOD0(MainThreadRun, void());
  MOCK_METHOD0(SecondaryThreadRun, void());
  MOCK_METHOD0(ThirdThreadRun, void());
  MOCK_METHOD0(ThreadJoined, void());
};

}  // namespace

TEST(RealThreadTest, Simple) {
  StrictMock<ThreadMock> mock;

  ExpectationSet calls;
  calls += EXPECT_CALL(mock, MainThreadRun()).Times(Exactly(1));
  calls += EXPECT_CALL(mock, SecondaryThreadRun()).Times(Exactly(1));
  EXPECT_CALL(mock, ThreadJoined()).Times(Exactly(1)).After(calls);

  {
    base::RealThread thread1(
        base::RealThread::kIoThread,
        [&mock](const base::Thread&) { mock.SecondaryThreadRun(); },
        std::unique_ptr<MessageLoop>(new MessageLoop()));
    mock.MainThreadRun();
  }
  mock.ThreadJoined();
}

TEST(RealThreadTest, TwoThreads) {
  ThreadMock mock;

  ExpectationSet calls_1, calls_2;

  auto& call = EXPECT_CALL(mock, MainThreadRun()).Times(Exactly(1));
  calls_1 += call;
  calls_2 += call;
  calls_1 += EXPECT_CALL(mock, SecondaryThreadRun()).Times(Exactly(1));
  calls_2 += EXPECT_CALL(mock, ThirdThreadRun()).Times(Exactly(1));
  calls_1 += EXPECT_CALL(mock, ThreadJoined()).After(calls_2);
  EXPECT_CALL(mock, ThreadJoined()).After(calls_1);

  {
    base::RealThread thread1(
        base::Thread::kIoThread,
        [&mock](const base::Thread&) { mock.SecondaryThreadRun(); },
        std::unique_ptr<MessageLoop>(new MessageLoop()));
    {
      base::RealThread thread2(
          base::Thread::kIoThread,
          [&mock](const base::Thread&) { mock.ThirdThreadRun(); },
          std::unique_ptr<MessageLoop>(new MessageLoop()));
      mock.MainThreadRun();
    }
    mock.ThreadJoined();
  }
  mock.ThreadJoined();
}

}  // namespace base
