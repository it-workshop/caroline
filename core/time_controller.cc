// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#include "core/time_controller.h"

namespace core {

TimeController::~TimeController() {}

time_t TimeController::GetGlobalTime() const {
  return ::time(nullptr);
}

}  // namespace core

