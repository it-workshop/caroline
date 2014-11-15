// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Mlodik Mikhail <mlodik_m@mail.ru>

#include "core/time_utils.h"

#include <cinttypes>
#include <ctime>
#include <fstream>  // NOLINT
#include <vector>
#include <string>

#include "base/logging.h"
#include "base/singleton.h"

INSTANCE_SINGLETON(core::TimeLog)

namespace core {

void TimeLog::AddEntry(std::string name, uint64_t time) {
  bool found_in_log = false;
  for (int i = 0; i < timetable_.size() && !found_in_log; i++) {
    if (!name.compare(timetable_.at(i).name)) {
      timetable_.at(i).data.push_back(time);
      found_in_log = true;
    }
  }
  if (!found_in_log) {
    Column column;
    column.name = name;
    column.data.push_back(time);
    timetable_.push_back(column);
  }
}

void TimeLog::FindAverage() {
  for (int i = 0; i < timetable_.size(); i++) {
    uint64_t sum = 0;
    for (int j = 0; j < timetable_.at(i).data.size(); j++)
      sum += timetable_.at(i).data.at(j);
    timetable_.at(i).average = sum / timetable_.at(i).data.size();
  }
}

void TimeLog::FindSum() {
  summary_.clear();
  int iter_number = 0;
  for (int i = 0; i < timetable_.size(); i++) {
    if (iter_number < timetable_.at(i).data.size())
      iter_number = timetable_.at(i).data.size();
  }
  for (int j = 0; j < iter_number; j++) {
    uint64_t sum = 0;
    for (int i = 0; i < timetable_.size(); i++) {
      if (j < timetable_.at(i).data.size())
        sum += timetable_.at(i).data.at(j);
    }
    summary_.push_back(sum);
  }
}

void TimeLog::PutToLog() {
  for (int i = 0; i < timetable_.size(); i++) {
    Column& column = timetable_.at(i);
    LOG(INFO) << column.name <<
      " is running of average of " <<
      column.average << " microseconds.\n";
  }
  for (int i = 0; i < summary_.size(); i++) {
    LOG(INFO) << "Iteration " << i << " ran for " <<
      summary_.at(i) << " microseconds.\n";
  }
}

TimeLog::~TimeLog() {
  FindAverage();
  FindSum();
  PutToLog();
}

Clock::Clock(std::string name) {
  start_moment_ = clock();
  name_ = name;
}

/// @returns time from working of ctor in us.
/// 1000000 means number of us in one second.
uint64_t Clock::Time() {
  clock_t curr_moment = clock();
  return static_cast<uint64_t>(curr_moment - StartMoment()) *
     static_cast<uint64_t>(1000000) / static_cast<uint64_t>(CLOCKS_PER_SEC);
}

void Clock::AddToTimeLog() {
  std::string name = GetName();
  uint64_t time = Time();
  core::TimeLog::GetInstance()->AddEntry(name, time);
}

}  // namespace core
