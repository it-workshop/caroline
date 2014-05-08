// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#include "core/predefined_position_controller.h"

#include "base/values.h"

namespace  core {

namespace {

const char kTimeLineNode[] = "timeline";
const char kPositionTimeNode[] = "time";
const char kPositionLocationNode[] = "location";
const char kPositionRotationNode[] = "rotation";

const size_t kLocationAxes = 3;
const size_t kRotationAxes = 4;

}  // namespace

// static
std::unique_ptr<PositionController>
PredefinedPositionController::Create(
    TimeController* time_controller, base::DictionaryValue* settings) {
  if (!settings)
    return std::unique_ptr<PositionController>();

  auto positions = base::ToList(settings->GetValue(kTimeLineNode));
  if (!positions)
    return std::unique_ptr<PositionController>();

  std::unique_ptr<PredefinedPositionController> controller(
      new PredefinedPositionController(time_controller));

  for (size_t i = 0, size = positions->size(); i < size; ++i) {
    auto position = base::ToDictionary(positions->GetValueAt(i));
    if (!position)
      continue;

    auto time = base::ToInteger(position->GetValue(kPositionTimeNode));
    auto location =
        base::ToList(position->GetValue(kPositionLocationNode));
    auto rotation =
        base::ToList(position->GetValue(kPositionRotationNode));

    if (!time || !location || !rotation
        || location->size() != kLocationAxes
        || rotation->size() != kRotationAxes)
      continue;

    auto l_x = base::ToFloat(location->GetValueAt(0));
    auto l_y = base::ToFloat(location->GetValueAt(1));
    auto l_z = base::ToFloat(location->GetValueAt(2));

    auto r_w = base::ToFloat(rotation->GetValueAt(0));
    auto r_x = base::ToFloat(rotation->GetValueAt(1));
    auto r_y = base::ToFloat(rotation->GetValueAt(2));
    auto r_z = base::ToFloat(rotation->GetValueAt(3));

    if (!l_x || !l_y || !l_z || !r_w || !r_x || !r_y || !r_z)
      continue;

    controller->time_mapped_position_.insert(std::make_pair(
        uint64_t(time->value()),
        Position(
            Point3D(
                l_x->value(),
                l_y->value(),
                l_z->value()),
            Quaternion(
                r_w->value(),
                r_x->value(),
                r_y->value(),
                r_z->value()))));
  }

  if (controller->time_mapped_position_.empty())
    return std::unique_ptr<PositionController>();

  controller->last_known_position_ = controller->time_mapped_position_.begin();

  return std::unique_ptr<PositionController>(controller.release());
}

PredefinedPositionController::PredefinedPositionController(
    TimeController* time_controller)
  : PositionController(time_controller) {
}

PredefinedPositionController::~PredefinedPositionController() {}

Point3D PredefinedPositionController::GetLocation() const {
  auto next_position = last_known_position_;
  ++next_position;
  for (uint64_t current_time = time_controller()->GetCurrentTime();
      last_known_position_->first < current_time
      && next_position != time_mapped_position_.end()
      && next_position->first <= current_time;
      ++next_position) {
    ++last_known_position_;
  }
  return last_known_position_->second.location();
}

Quaternion PredefinedPositionController::GetRotation() const {
  auto next_position = last_known_position_;
  ++next_position;
  for (uint64_t current_time = time_controller()->GetCurrentTime();
      last_known_position_->first < current_time
      && next_position != time_mapped_position_.end()
      && next_position->first <= current_time;
      ++next_position) {
    ++last_known_position_;
  }
  return last_known_position_->second.rotation();
}

}  // namespace  core
