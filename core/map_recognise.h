// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Sirotkin Dmitry <dmitriy.v.sirotkin@gmail.com
#ifndef CORE_MAP_RECOGNISE_H_
#define CORE_MAP_RECOGNISE_H_

#include "core/map_filter.h"
#include "opencv2/opencv.hpp"

namespace core {

class DepthMap;

class MapRecognise {
 public:
  MapRecognise();
  explicit MapRecognise(const DepthMap &map);

  ~MapRecognise() {}

  DepthMap filter(const DepthMap &map);
  void SetPrecision(float Precision);

 protected:
  DepthMap GetMap() { return new_map_; }
  void dfs(int PointWidth, int PointHeight, int NumberCounter);
  bool SmoothNeighbourhood(int PointWidth, int PointHeight,
                           int direction, float Precision);
  int NeighbourDepth(int PointWidth, int PointHeight, int direction);
  // Directions:
  // 0 - Left
  // 1 - Right
  // 2 - Down
  // 3 - Up
 private:
  float precision_;

  DepthMap object_map_;
  DepthMap new_map_;
};

}  // namespace core

#endif  // CORE_MAP_RECOGNISE_H_
