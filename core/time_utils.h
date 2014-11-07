// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Mlodik Mikhail <mlodik_m@mail.ru>

#ifndef CORE_TIME_UTILS_H_
#define CORE_TIME_UTILS_H_

#include <ctime>
#include <string>
#include <vector>

namespace core {

class TimeLog {
 public:
  TimeLog();
  void AddEntry(std::string name, double time);
  void FindAverage();
  void FindSum();
  bool PutToLog();
  void ChangeLogAddress(std::string address) { log_address_ = address; }
  ~TimeLog();
  struct Column {
    std::vector<double> data_;
    std::string name_;
    double average_;
  };

 private:
  std::vector<Column> timetable_;
  std::vector<double> summary_;
  std::string log_address_;
};

class Clock {
 public:
  Clock();
  explicit Clock(std::string name);
  clock_t StartMoment() { return start_moment_; }
  const double Time();
  void ChangeLogAddress(const std::string& address);
  void Log();
  const std::string GetName() { return name_; }
  TimeLog GetLog() { return time_log_; }
  virtual ~Clock() {}
 private:
  clock_t start_moment_;
  std::string name_;
 public:
  static TimeLog time_log_;
};

}  // namespace core

#endif  // CORE_TIME_UTILS_H_
