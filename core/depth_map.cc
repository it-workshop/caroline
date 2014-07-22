// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "core/depth_map.h"

#include <utility>

#include "core/cameras.h"
#include "core/triangulation.h"


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

std::unique_ptr<DepthMap> DepthMap::BuildMap(
    const OpticalFlow &flow,
    const Cameras &cam,
    int w, int h) {

  if ((w * h) != flow.size()) {
    return nullptr;
  }

  std::unique_ptr<DepthMap> map(new DepthMap(h, w));
  Triangulation triangulator;
  triangulator.SetCameraMatrices(cam);


  for (size_t i = 0; i < flow.size(); i++) {
    cv::Point2d p = flow.at(i).first;

    double depth = triangulator.TriangulateDepth(p, flow.at(i).second);

    map->SetDepth((int) p.x, (int) p.y, depth);
  }

  return map;
}

}  // namespace core
