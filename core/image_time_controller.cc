// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#include "core/image_time_controller.h"

#include "core/image_capture_impl.h"

namespace core {

ImageTimeController::ImageTimeController(int64_t fps)
  : fps_(fps ? fps : 1),
    us_delta_(1000000ul / (fps ? fps : 1)),
    current_time_(static_cast<int64_t>(0)) {}

ImageTimeController::~ImageTimeController() {}

uint64_t ImageTimeController::GetCurrentTime() const {
  return current_time_;
}

bool ImageTimeController::AddCapture(ImageCaptureImpl* capture) {
  if (capture->type() != ImageCapture::Type::IMAGE)
    return false;

  captures_.push_back(capture);
  return true;
}

bool ImageTimeController::Grab() {
  current_time_ += us_delta_;
  return TimeController::Grab();
}

}  // namespace core
