// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#include "core/video_time_controller.h"

#include "core/image_capture_impl.h"
#include "opencv2/highgui/highgui.hpp"

namespace core {

namespace {

const unsigned int kCurrentTimeMultiplicator = 1000;

}  // namespace

VideoTimeController::~VideoTimeController() {}

uint64_t VideoTimeController::GetCurrentTime() const {
  return current_time_;
}

bool VideoTimeController::AddCapture(ImageCaptureImpl* capture) {
  if (capture->type() != ImageCapture::Type::VIDEO &&
      capture->type() != ImageCapture::Type::CAPTURE)
    return false;

  captures_.push_back(capture);
  return true;
}

bool VideoTimeController::Grab() {
  bool first = true;
  for (auto capture : captures_) {
    if (!capture->GrabNextImage())
      return false;
    if (first)
      current_time_ = capture->capture()->get(cv::CAP_PROP_POS_MSEC);
    else if (current_time_ !=
        capture->capture()->get(cv::CAP_PROP_POS_MSEC))
      return false;
    first = false;
  }
  return !!current_time_;
}

}  // namespace core
