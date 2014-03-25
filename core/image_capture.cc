// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#include "core/image_capture.h"

#include "core/position_controller.h"

namespace core {

ImageCapture::ImageCapture(std::unique_ptr<PositionController>&& position_controller)
  : position_controller_(std::move(position_controller)) {}

ImageCapture::~ImageCapture() {}

}  // namespace core

