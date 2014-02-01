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
  Point3D(const int& x, const int& y, const int& z);

  void SetX(const int& x) { x_ = x; }
  void SetY(const int& y) { y_ = y; }
  void SetZ(const int& z) { z_ = z; }

  void Set(const int& x, const int& y, const int& z);

  int X() const { return x_; }
  int Y() const { return y_; }
  int Z() const { return z_; }

 private:
  int x_;
  int y_;
  int z_;
};

}  // namespace core

#endif  // CORE_POINT3D_H_
