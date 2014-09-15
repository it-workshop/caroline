// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#include "core/time_controller.h"

#include "core/image_capture_impl.h"

namespace core {

TimeController::~TimeController() {}

time_t TimeController::GetGlobalTime() const {
  return ::time(nullptr);
}

bool TimeController::Grab() {
  if (captures_.empty())
    return false;

  for (auto capture : captures_)
    if (!capture->GrabNextImage())
      return false;

  return true;
}

}  // namespace core

