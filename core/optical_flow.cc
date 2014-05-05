// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "core/optical_flow.h"

namespace core {

OpticalFlow::OpticalFlow()
{
}

void OpticalFlow::AddPoint(const cv::Point2d &x1, const cv::Point2d &x2) {
  image_one_.push_back(x1);
  image_two_.push_back(x2);
}

cv::Point2d OpticalFlow::ImageOnePoint(int num) const {
  if (num >= Size()) {
    return cv::Point2d(0, 0);
  }

  return image_one_.at(num);
}

cv::Point2d OpticalFlow::ImageTwoPoint(int num) const {
  if (num >= Size()) {
    return cv::Point2d(0, 0);
  }

  return image_two_.at(num);
}

int OpticalFlow::Size() const {
  return image_one_.size();
}

}  // namespace core
