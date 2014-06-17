// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include <utility>

#include "core/optical_flow.h"

namespace core {

OpticalFlow::OpticalFlow() {
}

void OpticalFlow::AddPoint(const cv::Point2d &x1, const cv::Point2d &x2) {
  optical_flow_.push_back(std::pair<cv::Point2d, cv::Point2d>(x1, x2));
}

cv::Point2d OpticalFlow::ImageOnePoint(int num) const {
  if (num >= Size()) {
    return cv::Point2d(0, 0);
  }

  return optical_flow_.at(num).first;
}

cv::Point2d OpticalFlow::ImageTwoPoint(int num) const {
  if (num >= Size()) {
    return cv::Point2d(0, 0);
  }

  return optical_flow_.at(num).second;
}

int OpticalFlow::Size() const {
  return optical_flow_.size();
}

}  // namespace core
