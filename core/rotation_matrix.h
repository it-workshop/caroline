// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Mlodik Mikhail <mlodik_m@mail.com>

#ifndef CORE_ROTATION_MATRIX_H_
#define CORE_ROTATION_MATRIX_H_

#include <vector>

#include "opencv2/core/mat.hpp"

namespace core {

class RotationMatrix {
 public:
  RotationMatrix();
  RotationMatrix(double angle, double axis_x, double axis_y, double axis_z);

  cv::Point3d Rotate(const cv::Point3d& point);

  virtual ~RotationMatrix();

  double RotationMatrixElement(int row, int column) {
    return a_.at(3 * row + column);
  }
  void SetMatrixElement(double value, int row, int column) {
    a_.at(3 * row + column) = value;
  }

 private:
  std::vector<double> a_;
};

}  // namespace core

#endif  // CORE_ROTATION_MATRIX_H_
