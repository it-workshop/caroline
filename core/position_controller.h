// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#ifndef CORE_POSITION_CONTROLLER_H_
#define CORE_POSITION_CONTROLLER_H_

namespace core {

class Point3D;
class Quaternion;

class PositionController {
 public:
  virtual ~PositionController();

  virtual Point3D GetLocation() const = 0;
  virtual Quaternion GetRotation() const = 0;
};

}  // namespace core

#endif  // CORE_POSITION_CONTROLLER_H_

