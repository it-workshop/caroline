// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Mlodik Mikhail <mlodik_m@mail.ru>

#ifndef CORE_TIME_UTILS_H_
#define CORE_TIME_UTILS_H_

#include <cinttypes>
#include <ctime>
#include <memory>
#include <string>
#include <vector>

#include "base/singleton.h"

namespace core {

/// @class TimeLog
/// There is only one instance of this class.
/// Collects, stores, manipulates, and outputs
/// time performance from class Clock.
class TimeLog : public base::Singleton<TimeLog> {
 public:
  TimeLog() {}

  /// Column is a column of log
  /// name is name of this column
  /// data is array of time performances of function
  /// with name "name" in microseconds
  /// average is average performance counted as mean value
  struct Column {
    std::vector<uint64_t> data;
    std::string name;
    uint64_t average;
  };

  /// Adds entry to the log.
  /// @param[in] name Name of column to which entry is added.
  /// @param[in] time Time in us
  void AddEntry(std::string name, uint64_t time);

  /// Finds average for all the columns in timetable.
  void FindAverage();
  /// Finds summary performance for each iteration.
  void FindSum();
  /// Writes average and summary to LOG(INFO)
  void PutToLog();

  std::vector<uint64_t> Summary() { return summary_; }
  std::vector<Column> Timetable() { return timetable_; }
  /// FindAverage(), FindSum(), and PutToLog() are made in destructor.
  virtual ~TimeLog();

 private:
  /// Array of columns.
  std::vector<Column> timetable_;
  /// Array for summary performance during iteration
  std::vector<uint64_t> summary_;
};

/// @class Clock. Counts time from creation to destruction.
/// Writes info about it to TimeLog.
class Clock {
 public:
  /// @param[in] name Name, under which time performance is
  /// written to TimeLog
  explicit Clock(std::string name);

  /// @returns starting moment in clock_t
  clock_t StartMoment() { return start_moment_; }
  /// @returns time passed since ctor worked in us.
  uint64_t Time();
  /// Adds entry with name "name_" and value "Time()" to TimeLog.
  void AddToTimeLog();
  const std::string GetName() { return name_; }
  /// Adds entry to TimeLog during destruction.
  virtual ~Clock() { AddToTimeLog(); }
 private:
  /// Moment of clock creation.
  clock_t start_moment_;
  /// Name of the clock.
  std::string name_;
};

}  // namespace core

#endif  // CORE_TIME_UTILS_H_
