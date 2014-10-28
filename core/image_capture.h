// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#ifndef CORE_IMAGE_CAPTURE_H_
#define CORE_IMAGE_CAPTURE_H_

#include <cinttypes>
#include <memory>

#include "core/position.h"

namespace cv {

class Mat;

}  // namespace cv

namespace core {

class ImageCapture {
 public:
  enum class Type {
    IMAGE,
    VIDEO,
    CAPTURE
  };

  explicit ImageCapture(Type type);
  virtual ~ImageCapture();

  virtual bool GrabNextImage() = 0;
  virtual cv::Mat GetNextImage() = 0;

  Type type() const { return type_; }

  virtual double GetFocusLength() const = 0;
  // Dots per meter.
  virtual int64_t GetDpm() const = 0;

  Position position() const { return position_; }
  void set_position(const Position& position) { position_ = position; }

 private:
  const Type type_;
  Position position_;
};

}  // namespace core

#endif  // CORE_IMAGE_CAPTURE_H_

