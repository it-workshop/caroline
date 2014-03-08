// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Konstantin Bormotov <bormotovk@gmail.com>

#include "opencv2/opencv.hpp"

#include "core/image_capture.h"

namespace core {
  ImageCapture::~ImageCapture() {}

  cv::Mat ImageCapture::getNextImage() {
    cv::Mat frame;
    cap >> frame;
    if (frame.empty()) {
      this->errorNumber = 2;
      exit(0);
    }
    return frame;
  }
  bool ImageCapture::hasNextImage() {
    if (errorNumber == 1) {
      std::cout << "Error when reading" << std::endl;
      return false;
    }
    if (errorNumber == 2) {
      std::cout << "Can't read frame" << std::endl;
      return false;
    }
    return true;
  }
}  // namespace core

