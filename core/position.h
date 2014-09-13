// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#ifndef CORE_POSITION_H_
#define CORE_POSITION_H_

#include "core/quaternion.h"
#include "opencv2/core/core.hpp"

namespace core {

class Position {
 public:
  Position(const cv::Point3d &location, const Quaternion& rotation);
  Position();
  virtual ~Position();

  cv::Point3d location() const {
    return location_;
  }
  Quaternion rotation() const {
    return rotation_;
  }

 private:
  cv::Point3d location_;
  Quaternion rotation_;
};

}  // namespace core

#endif  // CORE_POSITION_H_
