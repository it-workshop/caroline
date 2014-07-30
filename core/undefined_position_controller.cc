// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#include "core/undefined_position_controller.h"

namespace core {

// static
std::unique_ptr<PositionController>
UndefinedPositionController::Create(
    TimeController* time_controller, base::DictionaryValue* settings) {
// TODO(alex-ac): Implement this.
  return std::unique_ptr<PositionController>();
}

UndefinedPositionController::UndefinedPositionController(
    TimeController* time_controller)
  : PositionController(time_controller) {}

UndefinedPositionController::~UndefinedPositionController() {}

cv::Point3d UndefinedPositionController::GetLocation() const {
  return cv::Point3d();
}

Quaternion UndefinedPositionController::GetRotation() const {
  return Quaternion();
}

}  // namespace core
