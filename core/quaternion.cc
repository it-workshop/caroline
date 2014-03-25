// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#include "core/quaternion.h"

#include <cmath>

namespace core {

namespace {

double Normalize(double v, double x, double y, double z) {
  return v / ::sqrt(x * x + y * y + z * z);
}

}  // namespace

Quaternion::Quaternion()
  : w_(1.0),
    x_(0.0),
    y_(0.0),
    z_(0.0) {}

Quaternion::Quaternion(double a, double x, double y, double z)
  : w_(::cos(a / 2)),
    x_(::sin(a / 2) * Normalize(x, x, y, z)),
    y_(::sin(a / 2) * Normalize(y, x, y, z)),
    z_(::sin(a / 2) * Normalize(z, x, y, z)) {}

Quaternion::~Quaternion() {}

}  // namespace core

