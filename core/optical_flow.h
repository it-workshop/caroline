// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_OPTICAL_FLOW_H_
#define CORE_OPTICAL_FLOW_H_

#include <utility>
#include <vector>

#include "opencv2/opencv.hpp"

namespace core {

typedef std::vector<std::pair<cv::Point2d, cv::Point2d>> OpticalFlow;

}  // namespace core

#endif  // CORE_OPTICAL_FLOW_H_
