// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_OPTICAL_FLOW_H_
#define CORE_OPTICAL_FLOW_H_

#include <vector>

#include "opencv2/opencv.hpp"

namespace core {

class OpticalFlow
{
 public:
  OpticalFlow();

  void AddPoint(const cv::Point2d& x1, const cv::Point2d& x2);
  cv::Point2d ImageOnePoint(int num) const;
  cv::Point2d ImageTwoPoint(int num) const;

  int Size() const;

 private:
  std::vector<cv::Point2d> image_one_;
  std::vector<cv::Point2d> image_two_;
};

}  // namespace core

#endif // CORE_OPTICAL_FLOW_H_
