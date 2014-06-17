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
  enum class Type {
    IMAGE,
    VIDEO,
    CAPTURE
  };

  explicit ImageCapture(
      std::unique_ptr<PositionController>&& position_controller,
      Type type);
  virtual ~ImageCapture();

  PositionController* position_controller() const {
    return position_controller_.get();
  }

  virtual bool GrabNextImage() = 0;
  virtual cv::Mat GetNextImage() = 0;

  Type type() const { return type_; }

  virtual double GetFocusLength() const = 0;
  // Dots per meter.
  virtual int GetDpm() const = 0;

 private:
  std::unique_ptr<PositionController> position_controller_;
  const Type type_;
};

}  // namespace core

#endif  // CORE_IMAGE_CAPTURE_H_

