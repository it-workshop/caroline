// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#ifndef CORE_POSITION_H_
#define CORE_POSITION_H_

#include "core/point3d.h"
#include "core/quaternion.h"

namespace core {

class Position {
 public:
  Position(const Point3D& location, const Quaternion& rotation);
  Position();
  virtual ~Position();

  Point3D location() const {
    return location_;
  }
  Quaternion rotation() const {
    return rotation_;
  }

 private:
  Point3D location_;
  Quaternion rotation_;
};

}  // namespace core

#endif  // CORE_POSITION_H_
