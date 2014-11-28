// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#ifndef CORE_IMAGE_CAPTURE_MANAGER_H_
#define CORE_IMAGE_CAPTURE_MANAGER_H_

#include <cinttypes>
#include <vector>
#include <utility>

#include "core/image_capture.h"

namespace core {

class Position;
class TimeController;

class ImageCaptureManager {
 public:
  virtual ~ImageCaptureManager();

  /// Register preferences for manager.
  static void RegisterPreferences();

  static std::unique_ptr<ImageCaptureManager>
  Create();

  virtual ImageCapture::Type GetType() const = 0;
  virtual size_t GetCapturesCount() const = 0;
  virtual TimeController* GetTimeController() const = 0;
  virtual std::vector<std::pair<cv::Mat, Position>> GetNextFrameset() = 0;
  virtual const std::vector<ImageCapture*> GetCaptures() const = 0;
};

}  // namespace core

#endif  // CORE_IMAGE_CAPTURE_MANAGER_H_
