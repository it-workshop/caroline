// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "core/depth_map.h"

namespace core {

DepthMap::DepthMap(int height, int width) {
  height_ = height;
  width_ = width;

  depth_map_.resize(width_ * height_);

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      depth_map_[width_ * i + j] = kBadDepth;
    }
  }
}

void DepthMap::SetDepth(int x, int y, double depth) {
  if (x < 0 || y < 0) {
    return;
  }

  if ((x > width_) || (y > height_)) {
    return;
  }

  depth_map_[width_ * y + x] = depth;
}

double DepthMap::Depth(int x, int y) const {
  if (x < 0 || y < 0) {
    return kBadDepth;
  }

  if ((x > width_) || (y > height_)) {
    return kBadDepth;
  }

  return depth_map_[width_ * y + x];
}

}  // namespace core
