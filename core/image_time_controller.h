// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#ifndef CORE_IMAGE_TIME_CONTROLLER_H_
#define CORE_IMAGE_TIME_CONTROLLER_H_

#include "core/time_controller.h"

namespace core {

class ImageTimeController : public TimeController {
 public:
  explicit ImageTimeController(int64_t fps);
  virtual ~ImageTimeController();

  virtual uint64_t GetCurrentTime() const override;

  virtual bool AddCapture(ImageCaptureImpl* capture) override;

  virtual bool Grab() override;

 private:
  const uint64_t us_delta_;
  const uint64_t fps_;
  uint64_t current_time_;
};

}  // namespace core

#endif  // CORE_IMAGE_TIME_CONTROLLER_H_
