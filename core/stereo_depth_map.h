// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_STEREO_DEPTH_MAP_H_
#define CORE_STEREO_DEPTH_MAP_H_

#include "opencv2/opencv.hpp"

#include <string>

namespace core {
class StereoDepthMap {
 public:
  explicit StereoDepthMap() {}

  cv::Mat BuildMap(const std::string& leftImage, const std::string& rightImage);
};

}  // namespace core



#endif // CORE_STEREO_DEPTH_MAP_H_
