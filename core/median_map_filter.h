// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Sirotkin Dmitry <dmitriy.v.sirotkin@gmail.com

#ifndef CORE_MEDIAN_MAP_FILTER_H_
#define CORE_MEDIAN_MAP_FILTER_H_

#include "core/map_filter.h"
#include "opencv2/opencv.hpp"

namespace core {

class MedianMapFilter : public MapFilter {
 public:
  MedianMapFilter();

  ~MedianMapFilter() {}

  DepthMap filter(const DepthMap &map);
  void SetKernel(int KernelSize);

 private:
  int KernelSize_;
};

}  // namespace core

#endif  // CORE_MEDIAN_MAP_FILTER_H_
