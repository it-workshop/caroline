// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_POINT_CLOUD_H_
#define CORE_POINT_CLOUD_H_

#include <vector>

#include "core/point3d.h"

namespace core {

class PointCloud {
 public:
  PointCloud();

  void AddPoint(const Point3D& point) { cloud_.push_back(point); }

  int Size() const { return cloud_.size(); }

  std::vector<Point3D> Points() const { return cloud_; }

 private:
  std::vector<Point3D> cloud_;
};

}  // namespace core

#endif  // CORE_POINT_CLOUD_H_
