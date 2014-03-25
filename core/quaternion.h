// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#ifndef CORE_QUATERNION_H_
#define CORE_QUATERNION_H_

namespace core {

class Quaternion {
 public:
  Quaternion();
  Quaternion(double a, double x, double y, double z);
  virtual ~Quaternion();

  double w() const { return w_; }
  double x() const { return x_; }
  double y() const { return y_; }
  double z() const { return z_; }

 private:
  double w_;
  double x_;
  double y_;
  double z_;
};

}  // namespace core

#endif  // CORE_QUATERNION_H_

