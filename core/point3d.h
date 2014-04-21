// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_POINT3D_H_
#define CORE_POINT3D_H_

namespace core {

class Point3D {
 public:
  Point3D();
  Point3D(double x, double y, double z);

  void set_x(double x) { x_ = x; }
  void set_y(double y) { y_ = y; }
  void set_z(double z) { z_ = z; }

  void Set(double x, double y, double z);

  double x() const { return x_; }
  double y() const { return y_; }
  double z() const { return z_; }

 private:
  double x_;
  double y_;
  double z_;
};

}  // namespace core

#endif  // CORE_POINT3D_H_
