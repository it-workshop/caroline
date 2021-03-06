// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#include "core/video_time_controller.h"

#include "core/image_capture_impl.h"
#include "opencv2/highgui/highgui.hpp"

#if CV_VERSION_MAJOR > 2 || (CV_VERSION_MAJOR == 2 && CV_VERSION_MINOR >= 4)
#define CV_CAP_PROP_POS_MSEC cv::CAP_PROP_POS_MSEC
#endif

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
      current_time_ = (int64_t) capture->capture()->get(CV_CAP_PROP_POS_MSEC);
    else if (current_time_ !=
        capture->capture()->get(CV_CAP_PROP_POS_MSEC))
      return false;
    first = false;
  }
  return !!current_time_;
}

}  // namespace core
