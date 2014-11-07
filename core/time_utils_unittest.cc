// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Mlodik Mikhail <mlodik_m@mail.ru>

#include <ctime>

#include "gtest/gtest.h"

#include "core/time_utils.h"

TEST(ClockTest, ClockTest) {
  for (int i = 0; i < 3; i++) {
    core::Clock TestClock0("0-clock");
    TestClock0.Log();
    time_t time1 = time(nullptr);
    while (time(nullptr) == time1) {}
    core::Clock TestClock("1-clock");
    while (time(nullptr) == time1 + 1) {}
    TestClock.Log();
  }
  EXPECT_EQ(0, 0);
}
