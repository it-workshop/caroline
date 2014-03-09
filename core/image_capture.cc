// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Konstantin Bormotov <bormotovk@gmail.com>

#include "opencv2/opencv.hpp"

#include "core/image_capture.h"

namespace core {

ImageCapture::~ImageCapture() {}

cv::Mat ImageCapture::GetNextImage() {
  cv::Mat frame;
  if (cap_.read(frame)) {
    return cv::Mat();
  }
  cap_ >> frame;
  return frame;
}

bool ImageCapture::HasNextImage() const {
  if (error_ == true) {
    std::cout << "error_ when reading" << std::endl;
    return false;
  }
  return true;
}

}  // namespace core

