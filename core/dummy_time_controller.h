// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#ifndef CORE_DUMMY_TIME_CONTROLLER_H_
#define CORE_DUMMY_TIME_CONTROLLER_H_

#include "core/time_controller.h"

namespace core {

class DummyTimeController : public TimeController {
 public:
  virtual ~DummyTimeController();

  virtual uint64_t GetCurrentTime() const override;
  virtual bool AddCapture(ImageCaptureImpl* capture) override;
};

}  // namespace core

#endif  // CORE_DUMMY_TIME_CONTROLLER_H_

