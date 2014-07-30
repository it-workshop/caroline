// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_DEPTH_MAP_H_
#define CORE_DEPTH_MAP_H_

#include <vector>
#include <memory>
#include <utility>

#include "core/optical_flow.h"

namespace core {

class Cameras;

class DepthMap {
 public:
  static const int kBadDepth = -1;

  DepthMap(int width, int height);

  ~DepthMap() {}

  int width() const { return width_; }
  int height() const { return height_; }

  std::vector<double> depthVector() const { return depth_map_; }

  void SetDepth(int x, int y, double depth);
  double Depth(int x, int y) const;

  static std::unique_ptr<DepthMap> BuildMap(
      const std::vector<std::pair<cv::Point2d, cv::Point2d>>& flow,
      const Cameras& cam, int w, int h);

  std::vector<double>::const_iterator begin() const {
    return depth_map_.begin();
  }
  std::vector<double>::const_iterator end() const {
    return depth_map_.end();
  }

 private:
  int width_;
  int height_;


  std::vector<double> depth_map_;
};

}  // namespace core

#endif  // CORE_DEPTH_MAP_H_
