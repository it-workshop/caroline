// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "core/point3d.h"

namespace core {

Point3D::Point3D() :
  x_(0),
  y_(0),
  z_(0) {}

Point3D::Point3D(const double &x, const double &y, const double &z) :
  x_(x),
  y_(y),
  z_(z) {}

void Point3D::Set(const double& x, const double& y, const double& z) {
  SetX(x);
  SetY(y);
  SetZ(z);
}

}  // namespace core
