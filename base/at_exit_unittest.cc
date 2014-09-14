// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// @author Aleksandr Derbenev <13alexac@gmail.com>

#include "base/at_exit.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace {

class Mock {
 public:
  MOCK_METHOD0(Callback, void());
};

}  // namespace

TEST(AtExitManagerTest, SingletonCorrect) {
  base::AtExitManager manager;
  EXPECT_EQ(&manager, base::AtExitManager::GetInstance());
  {  // NOLINT
    base::AtExitManager second_manager;
    EXPECT_EQ(&second_manager, base::AtExitManager::GetInstance());
  }
  EXPECT_EQ(&manager, base::AtExitManager::GetInstance());
}

TEST(AtExitManagerTest, CallbackCall) {
  Mock mock;
  EXPECT_CALL(mock, Callback());
  {  // NOLINT
    base::AtExitManager manager;
    base::AtExitManager::GetInstance()->RegisterCallback(
        [&mock] () { mock.Callback(); });
  }
}

TEST(AtExitManagerTest, CallbacksCallsOrder) {
  Mock mocks[10];

  {  // NOLINT
    testing::InSequence dummy;
    for (auto it = mocks + 9, end = mocks - 1; it != end; --it)
      EXPECT_CALL(*it, Callback());
  }

  {  // NOLINT
    base::AtExitManager manager;
    for (auto it = std::begin(mocks), end = std::end(mocks); it != end; ++it)
      base::AtExitManager::GetInstance()->RegisterCallback(
          [it] () { it->Callback(); });
  }
}
