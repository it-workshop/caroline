// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#include "core/dummy_time_controller.h"

namespace core {

DummyTimeController::~DummyTimeController() {}

uint64_t DummyTimeController::GetCurrentTime() const { return 0; }

bool DummyTimeController::AddCapture(ImageCaptureImpl* capture) {
  captures_.push_back(capture);
  return true;
}

}  // namespace core

