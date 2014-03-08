// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Konstantin Bormotov <bormotovk@gmail.com>

#ifndef CORE_IMAGE_CAPTURE_H_
#define CORE_IMAGE_CAPTURE_H_

#include "opencv2/opencv.hpp"

namespace core {

  // For capturing images from camera or file
class ImageCapture {
 public:
  // source_name is file or camera id
  template <typename T>
  explicit ImageCapture(const T& source_name) {
    cap_ = cv::VideoCapture(source_name);
    error_ = false;
    if (!cap_.isOpened())
      error_ = true;
  }
  virtual ~ImageCapture();

  // Can return empty matrix.
  cv::Mat GetNextImage();
  bool HasNextImage() const;

 private:
  bool error_;
  cv::VideoCapture cap_;
};

}  // namespace core

#endif  // CORE_IMAGE_CAPTURE_H_
