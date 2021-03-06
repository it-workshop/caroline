// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#ifndef CORE_OPTICAL_FLOW_PROCESSOR_H_
#define CORE_OPTICAL_FLOW_PROCESSOR_H_

#include <memory>
#include <utility>
#include <vector>

#include "opencv2/core/core.hpp"

namespace cv {

class Mat;

}  // namespace cv

namespace core {

class OpticalFlowProcessor {
 public:
  static std::unique_ptr<OpticalFlowProcessor>
  Create();
  virtual ~OpticalFlowProcessor() {}

  virtual std::vector<std::pair<cv::Point2d, cv::Point2d>> Process(
      const cv::Mat& first,
      const cv::Mat& second) const = 0;

  static bool RegisterPreferences();
};

}  // namespace core

#endif  // CORE_OPTICAL_FLOW_PROCESSOR_H_
