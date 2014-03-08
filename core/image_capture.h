// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Konstantin Bormotov <bormotovk@gmail.com>
#ifndef CORE_IMAGE_CAPTURE_H_
#define CORE_IMAGE_CAPTURE_H_

#include "opencv2/opencv.hpp"

namespace core {
class ImageCapture {
 private:
  int errorNumber;
  cv::VideoCapture cap;
 public:
  template <typename T> ImageCapture(T sourceName) {
    cap = cv::VideoCapture(sourceName);
    errorNumber = 0;
    if (!cap.isOpened())
      errorNumber = 1;
  }
  virtual ~ImageCapture();
  cv::Mat getNextImage();
  bool hasNextImage();
};
}  // namespace core

#endif  // CORE_IMAGE_CAPTURE_H_
