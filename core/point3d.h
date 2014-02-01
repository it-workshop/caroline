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
  Point3D(const double &x, const double &y, const double &z);

  void SetX(const double& x) { x_ = x; }
  void SetY(const double& y) { y_ = y; }
  void SetZ(const double& z) { z_ = z; }

  void Set(const double& x, const double& y, const double& z);

  double X() const { return x_; }
  double Y() const { return y_; }
  double Z() const { return z_; }

 private:
  double x_;
  double y_;
  double z_;
};

}  // namespace core

#endif  // CORE_POINT3D_H_
