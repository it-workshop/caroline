// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#include "core/image_capture_manager_impl.h"

#include <algorithm>
#include <string>
#include <vector>
#include <utility>

#include "base/logging.h"
#include "core/dummy_time_controller.h"
#include "core/image_capture_impl.h"
#include "core/image_time_controller.h"
#include "core/position.h"
#include "core/preferences_service.h"
#include "core/video_time_controller.h"
#include "opencv2/core/core.hpp"
#include "json/value.h"

namespace core {

namespace {

// {
const char kCapturesNode[] = "captures";
//   "time": {
const char kTimeSettingsNode[] = "time";
const char kTimeControllerNode[] = "time.controller_type";
const char kTimeFpsNode[] = "time.fps";
//     "controller_type":
const char kTimeControllerTypeImage[] = "image";
const char kTimeControllerTypeVideo[] = "video";
const char kTimeControllerTypeDummy[] = "dummy";
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
void ImageCaptureManager::RegisterPreferences() {
  PrefService* prefs = PrefService::GetInstance();
  DCHECK(prefs);

  DCHECK(prefs->RegisterDict(kTimeSettingsNode));
  DCHECK(prefs->RegisterString(kTimeControllerNode,
                               kTimeControllerTypeDummy));
  DCHECK(prefs->RegisterInt(kTimeFpsNode, 1));

  DCHECK(prefs->RegisterList(kCapturesNode));
}

// static
std::unique_ptr<ImageCaptureManager>
ImageCaptureManager::Create() {
  std::unique_ptr<ImageCaptureManager> manager;

  PrefService* prefs = PrefService::GetInstance();
  DCHECK(prefs);

  std::unique_ptr<TimeController> time_controller;

  const std::string& time_controller_type =
      prefs->GetString(kTimeControllerNode);
  const int fps = prefs->GetInt(kTimeFpsNode);
  if (time_controller_type == kTimeControllerTypeImage)
    time_controller.reset(new ImageTimeController(fps));
  else if (time_controller_type == kTimeControllerTypeVideo)
    time_controller.reset(new VideoTimeController());
  else if (time_controller_type == kTimeControllerTypeDummy)
    time_controller.reset(new DummyTimeController());

  DCHECK(time_controller) << "Unknown time controller type preference.";

  Json::Value* captures_list = prefs->GetList(kCapturesNode);

  std::vector<std::unique_ptr<ImageCapture>> captures;
  for (const Json::Value& capture : *captures_list) {
    if (!capture.isObject()) {
      LOG(WARNING) << "Capture preference must be an object.";
      continue;
    }

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

    std::unique_ptr<ImageCaptureImpl> image_capture;
    if (capture_type_string == kCaptureTypeDevice && capture_id_present)
      image_capture.reset(
          new ImageCaptureImpl(capture_id_int));

    if (!dpm_present && dpi_present)
      dpm_int = dpi_int * kDpiToDpmMultiplier;

    if (!image_capture && capture_type_string == kCaptureTypeVideo &&
        !capture_source_string.empty()) {
      image_capture.reset(
          new ImageCaptureImpl(
              ImageCapture::Type::VIDEO, capture_source_string));
      image_capture->set_dpm(dpm_int);
      image_capture->set_focus_length(focus_length_present ?
          focus_length_double : kDefaultFocusLength);
    }

    if (!image_capture && capture_type_string == kCaptureTypeImage &&
        !capture_source_string.empty()) {
      image_capture.reset(
          new ImageCaptureImpl(
              ImageCapture::Type::IMAGE, capture_source_string));
      image_capture->set_dpm(dpm_int);
      image_capture->set_focus_length(focus_length_present ?
          focus_length_double : kDefaultFocusLength);
    }

    if (image_capture) {
      time_controller->AddCapture(image_capture.get());
      captures.push_back(std::move(image_capture));
    }
  }

  if (!captures.empty())
    manager.reset(new ImageCaptureManagerImpl(
      std::move(time_controller), std::move(captures), captures[0]->type()));

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
            capture->position());
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
