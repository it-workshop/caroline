// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#ifndef CORE_IMAGE_CAPTURE_H_
#define CORE_IMAGE_CAPTURE_H_

#include <memory>

namespace cv {

class Mat;

}  // namespace cv

namespace core {

class PositionController;

class ImageCapture {
 public:
  explicit ImageCapture(
      std::unique_ptr<PositionController>&& position_controller);
  virtual ~ImageCapture();

  PositionController* position_controller() const {
    return position_controller_.get();
  }

  virtual bool GrabNextImage() = 0;
  virtual cv::Mat GetNextImage() = 0;

 private:
  std::unique_ptr<PositionController> position_controller_;
};

}  // namespace core

#endif  // CORE_IMAGE_CAPTURE_H_

