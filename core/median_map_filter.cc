// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Sirotkin Dmitry <dmitriy.v.sirotkin@gmail.com
#include <vector>
#include "core/median_map_filter.h"
#include "core/depth_map.h"

namespace core {
MedianMapFilter::MedianMapFilter() {}

/*cv::Mat MedianMapFilter::matrisate(const DepthMap &map) {
  std::vector<double> vec = map.depthVector();
  cv::Mat matDepthVector(map.width(), map.height(), CV_8U);
  for (int i = 0; i < map.width(); ++i)
       for (int j = 0; j < map.height(); ++j) {
         matDepthVector.at<int>(i, j) = vec[map.width() * j + i];
       }
  return matDepthVector;
}

DepthMap MedianMapFilter::dematrisate(const cv::Mat &matDepthVector) {
  DepthMap vec(matDepthVector.rows, matDepthVector.cols);
  for (int i = 0; i < vec.width(); ++i)
       for (int j = 0; j < vec.height(); ++j) {
          vec.SetDepth(i, j, matDepthVector.at<int>(i, j) );
       }
  return vec;
}*/

DepthMap MedianMapFilter::filter(const DepthMap &map) {
  cv::Mat mat(map.width(), map.height(), CV_32F);
  for (int i = 0; i < map.width(); i++) {
    for (int j = 0; j < map.height(); j++) {
      mat.at<float>(i, j, 0) = map.Depth(i, j);
    }
  }

  cv::Mat smooth = mat.clone();

  for (int i = 1; i <= KernelSize_; i = i + 2)
    cv::medianBlur(mat, smooth, i);

  DepthMap newMap(map.width(), map.height());

  for (int i = 0; i < map.width(); i++) {
    for (int j = 0; j < map.height(); j++) {
      newMap.SetDepth(i, j, smooth.at<float>(i, j));
    }
  }
  return newMap;
}
  void MedianMapFilter::SetKernel(int KernelSize) {
    KernelSize_ = KernelSize;
  }
}  // namespace core
