// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#include "core/position_controller.h"

#include <string>

#include "base/values.h"
#include "core/predefined_position_controller.h"
#include "core/undefined_position_controller.h"

namespace core {

namespace {

const char kControllerTypeNode[] = "type";
const char kControllerTypePredefined[] = "predefined";
const char kControllerTypeUndefined[] = "undefined";

}  // namespace

// static
std::unique_ptr<PositionController>
PositionController::Create(TimeController* time_controller,
    base::DictionaryValue* settings) {
  if (!settings)
    return std::unique_ptr<PositionController>();

  auto controller_type =
      base::ToString(settings->GetValue(kControllerTypeNode));
  if (!controller_type)
    return std::unique_ptr<PositionController>();
  std::string controller_type_string = controller_type->value();

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
