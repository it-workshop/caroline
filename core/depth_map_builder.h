// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_DEPTH_MAP_BUILDER_H_
#define CORE_DEPTH_MAP_BUILDER_H_

#include "opencv2/opencv.hpp"

namespace core {

class Triangulation
{
 public:
  Triangulation();


};

class DepthMapBuilder
{
 public:
  DepthMapBuilder();

 protected:
  cv::Mat_<double> LinearTriangulation(cv::Point3d x1, cv::Matx34d P1, cv::Point3d x2, cv::Matx34d P2);
  cv::Mat_<double> IterativeTriangulation(cv::Point3d x1, cv::Matx34d P1, cv::Point3d x2, cv::Matx34d P2);


};

}  // namespace core

#endif // CORE_DEPTH_MAP_BUILDER_H_
