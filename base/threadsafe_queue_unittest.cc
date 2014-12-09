// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#include "base/threadsafe_queue.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::StrictMock;
using ::testing::InSequence;
using ::testing::Eq;
using ::testing::Exactly;

namespace {

int kThreadsCount = 10;
int kMessagesPerThread = 1;

class Mock {
 public:
  MOCK_METHOD1(Message, void(bool value));
};

}  // namespace

TEST(ThreadSafeQueueTest, SingleEnqueueDequeue) {
  base::ThreadSafeQueue<bool> queue;

  queue.Enqueue(true);
  bool result = false;
  EXPECT_TRUE(queue.Dequeue(&result));
  EXPECT_TRUE(result);
}

TEST(ThreadSafeQueueTest, Underflow) {
  base::ThreadSafeQueue<bool> queue;

  bool result = false;
  EXPECT_FALSE(queue.Dequeue(&result));
  EXPECT_FALSE(result);
  EXPECT_FALSE(queue.Dequeue(&result));
  EXPECT_FALSE(result);
}

TEST(ThreadSafeQueueTest, EnqueueAfterUnderflow) {
  base::ThreadSafeQueue<bool> queue;
  bool result = false;
  EXPECT_FALSE(queue.Dequeue(&result));
  EXPECT_FALSE(result);
  queue.Enqueue(true);
  EXPECT_TRUE(queue.Dequeue(&result));
  EXPECT_TRUE(result);
}

TEST(ThreadSafeQueueTest, SerrialEnqueue) {
  base::ThreadSafeQueue<int> queue;

  for (int i = 0; i < 10000; ++i) {
    queue.Enqueue(i);
  }

  int result = 0;
  for (int i = 0; i < 10000; ++i) {
    EXPECT_TRUE(queue.Dequeue(&result));
    EXPECT_EQ(i, result);
  }
  EXPECT_FALSE(queue.Dequeue(&result));
  EXPECT_EQ(9999, result);
}
