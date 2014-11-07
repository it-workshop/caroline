// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Mlodik Mikhail <mlodik_m@mail.ru>

#include "core/time_utils.h"

#include <ctime>
#include <fstream>  // NOLINT
#include <vector>
#include <string>

#include "base/path.h"

namespace core {

TimeLog::TimeLog() {
  log_address_ = "TimeLog.txt";
//  timetable_ = new std::vector<std::vector<double>>;
//  name_ = new std::vector<std::string>;
//  average_ = new std::vector<double>;
//  summary_ = new std::vector<double>;
}

void TimeLog::AddEntry(std::string name, double time) {
  bool FoundInLog = false;
  for (int i = 0; i < timetable_.size() && !FoundInLog; i++) {
    if (!name.compare(timetable_.at(i).name_)) {
      timetable_.at(i).data_.push_back(time);
      FoundInLog = true;
    }
  }
  if (!FoundInLog) {
    Column column;
    column.name_ = name;
    column.data_.push_back(time);
    timetable_.push_back(column);
  }
}

void TimeLog::FindAverage() {
  for (int i = 0; i < timetable_.size(); i++) {
    double sum = 0;
    for (int j = 0; j < timetable_.at(i).data_.size(); j++)
      sum += timetable_.at(i).data_.at(j);
    timetable_.at(i).average_ = sum / timetable_.at(i).data_.size();
  }
}

void TimeLog::FindSum() {
  summary_.clear();
  int iter_number = 0;
  for (int i = 0; i < timetable_.size(); i++) {
    if (iter_number < timetable_.at(i).data_.size())
      iter_number = timetable_.at(i).data_.size();
  }
  for (int j = 0; j < iter_number; j++) {
    double sum = 0;
    for (int i = 0; i < timetable_.size(); i++) {
      if (j < timetable_.at(i).data_.size())
        sum += timetable_.at(i).data_.at(j);
    }
    summary_.push_back(sum);
  }
}

bool TimeLog::PutToLog() {
  std::fstream file;
  file.open(log_address_, std::fstream::out);
  if (!file.is_open())
    return false;
  for (int i = 0; i < timetable_.size(); i++) {
    std::string str;
    str.append(timetable_.at(i).name_);
    str.append(" is running for average of ");
    str.append(std::to_string(timetable_.at(i).average_));
    str.append(" seconds.\n");
    file.write(str.c_str(), str.size());
  }
  for (int i = 0; i < summary_.size(); i++) {
    std::string str;
    str.append("Iteration ");
    str.append(std::to_string(i+1));
    str.append(" ran for ");
    str.append(std::to_string(summary_.at(i)));
    str.append(" seconds.\n");
    file.write(str.c_str(), str.size());
  }
  if (file.fail() || file.bad())
    return false;

  file.close();
  return true;
}

TimeLog::~TimeLog() {
  FindAverage();
  FindSum();
  PutToLog();
}

TimeLog Clock::time_log_;

Clock::Clock() {
  start_moment_ = clock();
}

Clock::Clock(std::string name) {
  start_moment_ = clock();
  name_ = name;
}

const double Clock::Time() {
  clock_t curr_moment = clock();
  return static_cast<double>(curr_moment - StartMoment()) / CLOCKS_PER_SEC;
}

void Clock::ChangeLogAddress(const std::string &address) {
  Clock::time_log_.ChangeLogAddress(address);
}

void Clock::Log() {
  std::string name = GetName();
  double time = Time();
  time_log_.AddEntry(name, time);
}

}  // namespace core
