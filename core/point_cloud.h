// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_POINT_CLOUD_H_
#define CORE_POINT_CLOUD_H_

#include <vector>

#include "opencv2/core/core.hpp"

namespace core {

class PointCloud {
 public:
  PointCloud();

  void AddPoint(const cv::Point3d& point) { cloud_.push_back(point); }

  int Size() const { return cloud_.size(); }

  std::vector<cv::Point3d> Points() const { return cloud_; }

 private:
  std::vector<cv::Point3d> cloud_;
};

}  // namespace core

#endif  // CORE_POINT_CLOUD_H_
