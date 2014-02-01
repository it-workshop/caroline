// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_DEPTH_MAP_H_
#define CORE_DEPTH_MAP_H_

#include <vector>

namespace core {

const int bad_depth = -1;

class DepthMap {
 public:
  DepthMap(const int& height, const int &width);

  int Height() const { return height_; }
  int Width() const { return width_; }

  void SetDepth(const int& x, const int& y, const double& depth);
  double Depth(const int& x, const int& y) const;

 private:
  int height_;
  int width_;

  std::vector<std::vector<double> > depth_map_;
};

}  // namespace core

#endif  // CORE_DEPTH_MAP_H_
