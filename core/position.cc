// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#include "core/position.h"

namespace core {

Position::Position(const cv::Point3d& location,
    const core::Quaternion& rotation)
  : location_(location),
    rotation_(rotation) {}

Position::Position() {}

Position::~Position() {}

}  // namespace core
