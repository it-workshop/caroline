// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "core/depth_map.h"

namespace core {

DepthMap::DepthMap(const int& height, const int&  width) {
  height_ = height;
  width_ = width;

  depth_map_.resize(height_);
  for (int i = 0; i < height_; i++) {
    depth_map_[i].resize(width_);
  }

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      depth_map_[i][j] = bad_depth;
    }
  }
}

void DepthMap::SetDepth(const int &x, const int &y, const double &depth) {
  if (x < 0 || y < 0) {
    return;
  }

  if ((x > width_) || (y > height_)) {
    return;
  }

  depth_map_[y][x] = depth;
}

double DepthMap::Depth(const int &x, const int &y) const {
  if (x < 0 || y < 0) {
    return bad_depth;
  }

  if ((x > width_) || (y > height_)) {
    return bad_depth;
  }

  return depth_map_[y][x];
}

}  // namespace core
