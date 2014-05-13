// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#include "core/image_capture_manager_impl.h"

#include <algorithm>
#include <string>
#include <vector>
#include <utility>

#include "base/values.h"
#include "core/config.h"
#include "core/image_capture_impl.h"
#include "core/image_time_controller.h"
#include "core/position_controller.h"
#include "core/video_time_controller.h"
#include "opencv2/core/mat.hpp"

namespace core {

namespace {

// {
const char kTimeSettingsNode[] = "time";
const char kCapturesNode[] = "captures";
//   "time": {
const char kTimeControllerNode[] = "controller_type";
const char kTimeFpsNode[] = "fps";
//     "controller_type":
const char kTimeControllerTypeImage[] = "image";
//   }
//   "captures": [
//   {
const char kCaptureTypeNode[] = "type";
const char kCaptureSourceNode[] = "source";
const char kCaptureIdNode[] = "id";
const char kCapturePositionSettingsNode[] = "position";
const char kCaptureFocusLengthNode[] = "focus_length";
const char kCaptureDpmNode[] = "dpm";
const char kCaptureDpiNode[] = "dpi";
//     "type":
const char kCaptureTypeDevice[] = "device";
const char kCaptureTypeVideo[] = "video";
const char kCaptureTypeImage[] = "image";
//   }
//   ]
// }

const double kDefaultFocusLength = 1.0;

const int kDefaultDpi = 96;
const double kDpiToDpmMultiplier = 39.37;

}  // namespace

// static
std::unique_ptr<ImageCaptureManager>
ImageCaptureManager::Create(Config* config) {
  std::unique_ptr<ImageCaptureManager> manager;
  if (!config)
    return manager;

  auto config_root = config->dictionary();
  if (!config_root)
    return manager;

  std::unique_ptr<TimeController> time_controller;
  ImageCapture::Type type;
  auto time_settings =
      base::ToDictionary(config_root->GetValue(kTimeSettingsNode));
  if (time_settings) {
    auto controller_type =
        base::ToString(time_settings->GetValue(kTimeControllerNode));
    if (controller_type) {
      const std::string& type_string = controller_type->value();
      if (type_string == kTimeControllerTypeImage) {
        auto fps = base::ToInteger(time_settings->GetValue(kTimeFpsNode));
        time_controller.reset(new ImageTimeController(fps ? fps->value() : 1));
      }
    }
  }
  if (!time_controller)
    time_controller.reset(new VideoTimeController());

  auto captures_list = base::ToList(config_root->GetValue(kCapturesNode));
  if (!captures_list || captures_list->empty())
    return manager;

  std::vector<std::unique_ptr<ImageCapture>> captures;
  for (int i = 0; i < captures_list->size(); ++i) {
    auto capture = base::ToDictionary(captures_list->GetValueAt(i));
    if (!capture)
      continue;

    auto capture_type = base::ToString(capture->GetValue(kCaptureTypeNode));
    const std::string& capture_type_string =
        capture_type ? capture_type->value() : std::string();
    auto capture_source = base::ToString(capture->GetValue(kCaptureSourceNode));
    const std::string& capture_source_string =
        capture_source ? capture_source->value() : std::string();
    auto capture_id = base::ToInteger(capture->GetValue(kCaptureIdNode));

    auto focus_length =
        base::ToFloat(capture->GetValue(kCaptureFocusLengthNode));

    auto dpm = base::ToInteger(capture->GetValue(kCaptureDpmNode));
    auto dpi = base::ToInteger(capture->GetValue(kCaptureDpiNode));

    auto position_settings =
        base::ToDictionary(capture->GetValue(kCapturePositionSettingsNode));

    auto position_controller = PositionController::Create(time_controller.get(),
        position_settings);

    std::unique_ptr<ImageCaptureImpl> image_capture;
    if (capture_type_string == kCaptureTypeDevice && capture_id)
      image_capture.reset(
          new ImageCaptureImpl(std::move(position_controller),
              capture_id->value()));

    int dpm_value = kDefaultDpi * kDpiToDpmMultiplier;
    if (!image_capture) {
      if (dpm)
        dpm_value = dpm->value();
      else if (dpi)
        dpm_value = dpi->value() * kDpiToDpmMultiplier;
    }

    if (!image_capture && capture_type_string == kCaptureTypeVideo &&
        !capture_source_string.empty()) {
      image_capture.reset(
          new ImageCaptureImpl(std::move(position_controller),
              ImageCapture::Type::VIDEO, capture_source_string));
      image_capture->set_dpm(dpm_value);
      image_capture->set_focus_length(focus_length ?
          focus_length->value() : kDefaultFocusLength);
    }

    if (!image_capture && capture_type_string == kCaptureTypeImage &&
        !capture_source_string.empty()) {
      image_capture.reset(
          new ImageCaptureImpl(std::move(position_controller),
              ImageCapture::Type::IMAGE, capture_source_string));
      image_capture->set_dpm(dpm_value);
      image_capture->set_focus_length(focus_length ?
          focus_length->value() : kDefaultFocusLength);
    }

    if (!image_capture)
      continue;
  }

  return manager;
}

ImageCaptureManagerImpl::ImageCaptureManagerImpl(
    std::unique_ptr<TimeController>&& time_controller,
    std::vector<std::unique_ptr<ImageCapture>>&& captures,
    ImageCapture::Type type)
  : time_controller_(std::move(time_controller)),
    captures_(std::move(captures)),
    type_(type) {}

ImageCaptureManagerImpl::~ImageCaptureManagerImpl() {}

ImageCapture::Type ImageCaptureManagerImpl::GetType() const {
  return type_;
}

size_t ImageCaptureManagerImpl::GetCapturesCount() const {
  return captures_.size();
}

TimeController* ImageCaptureManagerImpl::GetTimeController() const {
  return time_controller_.get();
}

std::vector<std::pair<cv::Mat, Position>>
ImageCaptureManagerImpl::GetNextFrameset() {
  std::vector<std::pair<cv::Mat, Position>> frameset(captures_.size());
  std::transform(captures_.begin(), captures_.end(), frameset.begin(),
      [] (const std::unique_ptr<ImageCapture>& capture)
      -> std::pair<cv::Mat, Position> {
        return std::make_pair(capture->GetNextImage(),
            capture->position_controller()->position());
      });
  return frameset;
}

const std::vector<ImageCapture*>
ImageCaptureManagerImpl::GetCaptures() const {
  std::vector<ImageCapture*> captures(captures_.size());
  std::transform(captures_.begin(), captures_.end(), captures.begin(),
      [] (const std::unique_ptr<ImageCapture>& capture)
      -> ImageCapture* {
        return capture.get();
      });
  return captures;
}

}  // namespace core
