// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Konstantin Bormotov <bormotovk@gmail.com>

#include "core/image_capture_impl.h"

#include "opencv2/opencv.hpp"

#include "core/position_controller.h"

#if CV_VERSION_MAJOR > 2 || (CV_VERSION_MAJOR == 2 && CV_VERSION_MINOR >= 4)
#define CV_CAP_PROP_FORMAT cv::CAP_PROP_FORMAT
#endif

namespace core {

ImageCaptureImpl::ImageCaptureImpl(
    std::unique_ptr<PositionController>&& position_controller,
    Type type,
    const std::string& source_name)
  : ImageCapture(std::move(position_controller), type),
    capture_(new cv::VideoCapture(source_name)) {
  if (!capture_->isOpened() || !capture_->set(CV_CAP_PROP_FORMAT, CV_8U))
    capture_.reset();
}

ImageCaptureImpl::ImageCaptureImpl(
    std::unique_ptr<PositionController>&& position_controller,
    int64_t camera_id)
  : ImageCapture(std::move(position_controller), Type::CAPTURE),
    capture_(new cv::VideoCapture(static_cast<int>(camera_id))) {
  if (!capture_->isOpened() || !capture_->set(CV_CAP_PROP_FORMAT, CV_8U))
    capture_.reset();
}

ImageCaptureImpl::~ImageCaptureImpl() {}

cv::Mat ImageCaptureImpl::GetNextImage() {
  cv::Mat frame;
  if (!capture_->retrieve(frame)) {
    return cv::Mat();
  }
  return frame;
}

bool ImageCaptureImpl::GrabNextImage() {
  return capture_ && capture_->grab();
}

double ImageCaptureImpl::GetFocusLength() const {
  return focus_length_;
}

int64_t ImageCaptureImpl::GetDpm() const {
  return dpm_;
}

}  // namespace core

