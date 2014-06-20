// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_DEPTH_MAP_H_
#define CORE_DEPTH_MAP_H_

#include <vector>

namespace core {

class OpticalFlow;
class Cameras;

class DepthMap {
 public:
  static const int kBadDepth = -1;

  DepthMap(int height, int width);

  ~DepthMap() {}

  int height() const { return height_; }
  int width() const { return width_; }

  void SetDepth(int x, int y, double depth);
  double Depth(int x, int y) const;

  static DepthMap *BuildMap(const OpticalFlow& flow,
                            const Cameras& cam,
                            int w, int h);

 private:
  int height_;
  int width_;

  std::vector<double> depth_map_;
};

}  // namespace core

#endif  // CORE_DEPTH_MAP_H_
