// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Mlodik Mikhail <mlodik_m@mail.com>

#ifndef CORE_ROTATION_MATRIX_H_
#define CORE_ROTATION_MATRIX_H_

#include "core/point3d.h"

namespace core {

class Rotation_Matrix {
 public:
  Rotation_Matrix();
  Rotation_Matrix(double angle, double axis_x, double axis_y, double axis_z);

  Point3D Rotate(const Point3D& point);

  virtual ~Rotation_Matrix();

  double RotationMatrixElement(int row, int column) { return a_[row][column];}

 private:
  double **a_;
};

}  // namespace core

#endif  // CORE_ROTATION_MATRIX_H_
