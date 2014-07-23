// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Mlodik Mikhail <mlodik_m@mail.com>

#include "core/rotation_matrix.h"
#include <cmath>

namespace core {

Rotation_Matrix::Rotation_Matrix() {
  a_ = new double* [3];
  for (int i = 0; i < 3; i++)
    a_[i] = new double[3];
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      a_[i][j] = (i == j) ? 1 : 0;
}


Rotation_Matrix::Rotation_Matrix(double angle, double x, double y, double z) {
  if ( (x == 0) && (y == 0) && (z == 0) ) {
    Rotation_Matrix r;
    *this = r;
  } else {
    a_ = new double* [3];
    for (int i = 0; i < 3; i++)
      a_[i] = new double[3];
    double *axis = new double[3] {x , y, z};
    double Norm = sqrt(x*x + y*y + z*z);
    for (int i = 0; i < 3; i++) axis[i] = axis[i] / Norm;
    double Cos = cos(angle);
    double Sin = sin(angle);
    a_[0][0] = axis[0]*axis[0]*(1 - Cos) + Cos;
    a_[0][1] = axis[0]*axis[1]*(1 - Cos) - axis[2]*Sin;
    a_[0][2] = axis[0]*axis[2]*(1 - Cos) + axis[1]*Sin;
    a_[1][0] = axis[1]*axis[0]*(1 - Cos) + axis[2]*Sin;
    a_[1][1] = axis[1]*axis[1]*(1 - Cos) + Cos;
    a_[1][2] = axis[1]*axis[2]*(1 - Cos) - axis[0]*Sin;
    a_[2][0] = axis[2]*axis[0]*(1 - Cos) - axis[1]*Sin;
    a_[2][1] = axis[2]*axis[1]*(1 - Cos) + axis[0]*Sin;
    a_[2][2] = axis[2]*axis[2]*(1 - Cos) + Cos;

    delete [] axis;
  }
}

Point3D Rotation_Matrix::Rotate(const Point3D& point) {
  Point3D new_point;
  double *radius_vector1 = new double[3] { point.x(), point.y(), point.z() };
  double *radius_vector2 = new double[3] { 0, 0, 0 };
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++)
      radius_vector2[i] +=
        this->RotationMatrixElement(i, j) * radius_vector1[j];
  }
  new_point.Set(radius_vector2[0], radius_vector2[1], radius_vector2[2]);
  delete []radius_vector1;
  delete []radius_vector2;
  return new_point;
}

Rotation_Matrix::~Rotation_Matrix() {
  for (int i = 0; i < 3; i++) delete [] a_[i];
  delete [] a_;
}

}  // namespace core
