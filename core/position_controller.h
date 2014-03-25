// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#ifndef CORE_POSITION_CONTROLLER_H_
#define CORE_POSITION_CONTROLLER_H_

namespace core {

class Point3D;
class Quaternion;

class TimeController;

class PositionController {
 public:
  explicit PositionController(TimeController* time_controller);
  virtual ~PositionController();

  virtual Point3D GetLocation() const = 0;
  virtual Quaternion GetRotation() const = 0;

  TimeController* time_controller() const {
    return time_controller_;
  }

  void set_time_controller(TimeController* time_controller) {
    time_controller_ = time_controller;
  }

 private:
  TimeController* time_controller_;
};

}  // namespace core

#endif  // CORE_POSITION_CONTROLLER_H_

