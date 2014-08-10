// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#include "core/predefined_position_controller.h"

#include "json/value.h"

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
    TimeController* time_controller, const Json::Value& settings) {
  if (!settings.isObject() || !settings.isMember(kTimeLineNode))
    return std::unique_ptr<PositionController>();

  const Json::Value& positions = settings[kTimeLineNode];
  if (!positions.isArray())
    return std::unique_ptr<PositionController>();

  std::unique_ptr<PredefinedPositionController> controller(
      new PredefinedPositionController(time_controller));

  for (auto it = positions.begin(), end = positions.end(); it != end; ++it) {
    const Json::Value& position = *it;
    if (!position.isObject()
        || !position.isMember(kPositionTimeNode)
        || !position.isMember(kPositionLocationNode)
        || !position.isMember(kPositionRotationNode))
      continue;

    const Json::Value& time = position[kPositionTimeNode];
    const Json::Value& location = position[kPositionLocationNode];
    const Json::Value& rotation = position[kPositionRotationNode];

    if (!time.isUInt64()
        || !location.isArray() || location.size() != kLocationAxes
        || !rotation.isArray() || rotation.size() != kRotationAxes)
      continue;

    const Json::Value& l_x_node = location[0];
    const Json::Value& l_y_node = location[1];
    const Json::Value& l_z_node = location[2];

    const Json::Value& r_w_node = rotation[0];
    const Json::Value& r_x_node = rotation[1];
    const Json::Value& r_y_node = rotation[2];
    const Json::Value& r_z_node = rotation[3];

    if (!l_x_node.isDouble()
        || !l_y_node.isDouble()
        || !l_z_node.isDouble()
        || !r_w_node.isDouble()
        || !r_x_node.isDouble()
        || !r_y_node.isDouble()
        || !r_z_node.isDouble())
      continue;

    double l_x = l_x_node.asDouble();
    double l_y = l_y_node.asDouble();
    double l_z = l_z_node.asDouble();

    double r_w = r_w_node.asDouble();
    double r_x = r_x_node.asDouble();
    double r_y = r_y_node.asDouble();
    double r_z = r_z_node.asDouble();

    controller->time_mapped_position_.insert(std::make_pair(
        time.asUInt64(),
        Position(cv::Point3d(l_x, l_y, l_z), Quaternion(r_w, r_x, r_y, r_z))));
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

cv::Point3d PredefinedPositionController::GetLocation() const {
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
