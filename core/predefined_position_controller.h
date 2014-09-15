// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#ifndef CORE_PREDEFINED_POSITION_CONTROLLER_H_
#define CORE_PREDEFINED_POSITION_CONTROLLER_H_

#include <map>
#include <memory>

#include "core/position_controller.h"

namespace Json {

class Value;

}  // namespace Json

namespace core {

class PredefinedPositionController : public PositionController {
 public:
  static std::unique_ptr<PositionController> Create(
      TimeController* time_controller, const Json::Value& settings);
  explicit PredefinedPositionController(TimeController* time_controller);
  virtual ~PredefinedPositionController();

  virtual cv::Point3d GetLocation() const override;
  virtual Quaternion GetRotation() const override;

 private:
  std::map<uint64_t, Position> time_mapped_position_;
  mutable std::map<uint64_t, Position>::const_iterator last_known_position_;
};

}  // namespace core

#endif  // CORE_PREDEFINED_POSITION_CONTROLLER_H_
