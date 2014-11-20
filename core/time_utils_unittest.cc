// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Mlodik Mikhail <mlodik_m@mail.ru>

#include <ctime>

#include "gtest/gtest.h"

#include "base/at_exit.h"
#include "core/time_utils.h"

TEST(TimeUtilsTest, TimeLogTest) {
  base::AtExitManager at_exit_manager;
  new core::TimeLog;
  core::TimeLog* log = core::TimeLog::GetInstance();
  log->AddEntry("first", 1000000);
  log->AddEntry("first", 1200000);
  log->AddEntry("first", 1400000);
  log->AddEntry("second", 3000000);
  log->AddEntry("second", 3200000);
  log->AddEntry("second", 3400000);
  log->FindAverage();
  log->FindSum();
  EXPECT_EQ(log->Timetable().size(), 2);
  EXPECT_EQ(log->Summary().size(), 3);
  EXPECT_EQ(log->Timetable().at(0).average, 1200000);
  EXPECT_EQ(log->Timetable().at(1).average, 3200000);
  EXPECT_EQ(log->Summary().at(0), 4000000);
  EXPECT_EQ(log->Summary().at(1), 4400000);
  EXPECT_EQ(log->Summary().at(2), 4800000);
}
