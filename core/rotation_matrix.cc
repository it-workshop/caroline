// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Mlodik Mikhail <mlodik_m@mail.com>

#include "core/rotation_matrix.h"

#include <cmath>

namespace core {

RotationMatrix::RotationMatrix() {
  a_.resize(9);
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      this->SetMatrixElement((i == j) ? 1 : 0, i, j);
}


RotationMatrix::RotationMatrix(double angle, double x, double y, double z) {
  if ( (x == 0) && (y == 0) && (z == 0) ) {
    RotationMatrix r;
    *this = r;
  } else {
    a_.resize(9);
    double axis[3] = {x, y, z};
    double Norm = sqrt(x*x + y*y + z*z);
    for (int i = 0; i < 3; i++) axis[i] = axis[i] / Norm;
    double Cos = cos(angle);
    double Sin = sin(angle);
    this->SetMatrixElement(axis[0]*axis[0]*(1 - Cos) + Cos, 0, 0);
    this->SetMatrixElement(axis[0]*axis[1]*(1 - Cos) - axis[2]*Sin, 0, 1);
    this->SetMatrixElement(axis[0]*axis[2]*(1 - Cos) + axis[1]*Sin, 0, 2);
    this->SetMatrixElement(axis[1]*axis[0]*(1 - Cos) + axis[2]*Sin, 1, 0);
    this->SetMatrixElement(axis[1]*axis[1]*(1 - Cos) + Cos, 1, 1);
    this->SetMatrixElement(axis[1]*axis[2]*(1 - Cos) - axis[0]*Sin, 1, 2);
    this->SetMatrixElement(axis[2]*axis[0]*(1 - Cos) - axis[1]*Sin, 2, 0);
    this->SetMatrixElement(axis[2]*axis[1]*(1 - Cos) + axis[0]*Sin, 2, 1);
    this->SetMatrixElement(axis[2]*axis[2]*(1 - Cos) + Cos, 2, 2);
  }
}

Point3D RotationMatrix::Rotate(const Point3D& point) {
  Point3D new_point;
  double radius_vector1[3] = {point.x(), point.y(), point.z()};
  double radius_vector2[3] = {0, 0, 0};
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++)
      radius_vector2[i] +=
        this->RotationMatrixElement(i, j) * radius_vector1[j];
  }
  new_point.Set(radius_vector2[0], radius_vector2[1], radius_vector2[2]);
  return new_point;
}

RotationMatrix::~RotationMatrix() {}

}  // namespace core
