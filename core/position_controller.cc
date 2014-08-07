// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#include "core/position_controller.h"

#include <string>

#include "core/predefined_position_controller.h"
#include "core/undefined_position_controller.h"
#include "json/value.h"

namespace core {

namespace {

const char kControllerTypeNode[] = "type";
const char kControllerTypePredefined[] = "predefined";
const char kControllerTypeUndefined[] = "undefined";

}  // namespace

// static
std::unique_ptr<PositionController>
PositionController::Create(TimeController* time_controller,
    const Json::Value& settings) {
  if (!settings.isObject()
      || settings.isMember(kControllerTypeNode))
    return std::unique_ptr<PositionController>();

  const Json::Value& controller_type = settings[kControllerTypeNode];
  if (!controller_type.isString())
    return std::unique_ptr<PositionController>();
  const std::string& controller_type_string = controller_type.asString();

  if (kControllerTypePredefined == controller_type_string)
    return PredefinedPositionController::Create(time_controller, settings);

  if (kControllerTypeUndefined == controller_type_string)
    return UndefinedPositionController::Create(time_controller, settings);

  return std::unique_ptr<PositionController>();
}

PositionController::PositionController(TimeController* time_controller)
  : time_controller_(time_controller) {}

PositionController::~PositionController() {}

void PositionController::UpdateCacheIfNeeded() const {
  if (current_time_ != time_controller_->GetCurrentTime()) {
    current_time_ = time_controller_->GetCurrentTime();
    location_ = GetLocation();
    rotation_ = GetRotation();
    position_ = Position(location_, rotation_);
  }
}

}  // namespace core
