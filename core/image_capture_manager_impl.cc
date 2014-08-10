// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#include "core/image_capture_manager_impl.h"

#include <algorithm>
#include <string>
#include <vector>
#include <utility>

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
ImageCaptureManager::Create(const Config* config) {
  std::unique_ptr<ImageCaptureManager> manager;
  if (!config)
    return manager;

  const Json::Value* config_root = config->dictionary();
  if (!config_root || config_root->isMember(kTimeSettingsNode))
    return manager;

  std::unique_ptr<TimeController> time_controller;
  const Json::Value& time_settings = (*config_root)[kTimeSettingsNode];
  if (time_settings.isObject() && time_settings.isMember(kTimeControllerNode)) {
    const Json::Value& controller_type = time_settings[kTimeControllerNode];
    if (controller_type.isString()) {
      const std::string& type_string = controller_type.asString();
      if (type_string == kTimeControllerTypeImage) {
        if (time_settings.isMember(kTimeFpsNode)) {
          const Json::Value& fps = time_settings[kTimeFpsNode];
          time_controller.reset(
              new ImageTimeController(fps.isUInt() ? fps.asUInt() : 1));
        } else {
          time_controller.reset(
              new ImageTimeController(1));
        }
      }
    }
  }
  if (!time_controller)
    time_controller.reset(new VideoTimeController());

  const Json::Value& captures_list = (*config_root)[kCapturesNode];
  if (!captures_list.isArray() || captures_list.empty())
    return manager;

  std::vector<std::unique_ptr<ImageCapture>> captures;
  for (auto it = captures_list.begin(),
      end = captures_list.end(); it != end; ++it) {
    const Json::Value& capture = *it;
    if (!capture.isObject())
      continue;

    std::string capture_type_string;
    if (capture.isMember(kCaptureTypeNode)) {
      const Json::Value& capture_type = capture[kCaptureTypeNode];
      if (capture_type.isString())
        capture_type_string = capture_type.asString();
    }

    std::string capture_source_string;
    if (capture.isMember(kCaptureSourceNode)) {
      const Json::Value& capture_source = capture[kCaptureSourceNode];
      if (capture_source.isString())
        capture_source_string = capture_source.asString();
    }

    uint64_t capture_id_int = 0;
    bool capture_id_present = false;
    if (capture.isMember(kCaptureIdNode)) {
      const Json::Value& capture_id = capture[kCaptureIdNode];
      if (capture_id.isUInt64()) {
        capture_id_present = true;
        capture_id_int = capture_id.asUInt64();
      }
    }

    double focus_length_double = 0;
    bool focus_length_present = false;
    if (capture.isMember(kCaptureFocusLengthNode)) {
      const Json::Value& focus_length = capture[kCaptureFocusLengthNode];
      if (focus_length.isDouble()) {
        focus_length_present = true;
        focus_length_double = focus_length.asDouble();
      }
    }

    uint64_t dpm_int = kDefaultDpi * kDpiToDpmMultiplier;
    bool dpm_present = false;
    if (capture.isMember(kCaptureDpmNode)) {
      const Json::Value& dpm = capture[kCaptureDpmNode];
      if (dpm.isUInt64()) {
        dpm_present = true;
        dpm_int = dpm.asUInt64();
      }
    }
    uint64_t dpi_int = kDefaultDpi;
    bool dpi_present = false;
    if (capture.isMember(kCaptureDpiNode)) {
      const Json::Value& dpi = capture[kCaptureDpiNode];
      if (dpi.isUInt64()) {
        dpi_present = true;
        dpi_int = dpi.asUInt64();
      }
    }

    std::unique_ptr<PositionController> position_controller;
    if (capture.isMember(kCapturePositionSettingsNode)) {
      const Json::Value& position_settings =
          capture[kCapturePositionSettingsNode];
      position_controller = PositionController::Create(
          time_controller.get(),
          position_settings);
    }

    std::unique_ptr<ImageCaptureImpl> image_capture;
    if (capture_type_string == kCaptureTypeDevice && capture_id_present)
      image_capture.reset(
          new ImageCaptureImpl(std::move(position_controller), capture_id_int));

    if (!dpm_present && dpi_present)
      dpm_int = dpi_int * kDpiToDpmMultiplier;

    if (!image_capture && capture_type_string == kCaptureTypeVideo &&
        !capture_source_string.empty()) {
      image_capture.reset(
          new ImageCaptureImpl(std::move(position_controller),
              ImageCapture::Type::VIDEO, capture_source_string));
      image_capture->set_dpm(dpm_int);
      image_capture->set_focus_length(focus_length_present ?
          focus_length_double : kDefaultFocusLength);
    }

    if (!image_capture && capture_type_string == kCaptureTypeImage &&
        !capture_source_string.empty()) {
      image_capture.reset(
          new ImageCaptureImpl(std::move(position_controller),
              ImageCapture::Type::IMAGE, capture_source_string));
      image_capture->set_dpm(dpm_int);
      image_capture->set_focus_length(focus_length_present ?
          focus_length_double : kDefaultFocusLength);
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
