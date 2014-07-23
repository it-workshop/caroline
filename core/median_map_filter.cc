// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Sirotkin Dmitry <dmitriy.v.sirotkin@gmail.com
#include <vector>
#include "core/median_map_filter.h"
#include "core/depth_map.h"



namespace core {
MedianMapFilter::MedianMapFilter() {}

cv::Mat MedianMapFilter::matrisate(const DepthMap &map) {
  std::vector<double> vec = map.depthVector();
  cv::Mat matDepthVector(map.height(), map.width(), CV_8U);
  for (int i = 0; i < map.height(); ++i)
       for (int j = 0; j < map.width(); ++j) {
         matDepthVector.at<int>(i, j) = vec[map.width() * i + j];
       }
  return matDepthVector;
}

DepthMap MedianMapFilter::dematrisate(const cv::Mat &matDepthVector) {
  DepthMap vec(matDepthVector.rows, matDepthVector.cols);
  for (int i = 0; i < vec.height(); ++i)
       for (int j = 0; j < vec.width(); ++j) {
          vec.SetDepth(i, j, matDepthVector.at<int>(i, j) );
       }
  return vec;
}

DepthMap MedianMapFilter::filter(const DepthMap &map) {
  cv::Mat mat(map.width(), map.height(), CV_32F);
  mat.setTo(228);
  for (int i = 0; i < map.height(); i++) {
    for (int j = 0; j < map.width(); j++) {
      mat.at<float>(j, i, 0) = map.Depth(j, i);
    }
  }

  cv::Mat smooth = mat.clone();

  for (int i = 1; i <= KernelSize_; i = i + 2)
    cv::medianBlur(mat, smooth, i);

  DepthMap newMap(map.height(), map.width());
  for (int i = 0; i < map.height(); i++) {
    for (int j = 0; j < map.width(); j++) {
      newMap.SetDepth(j, i, smooth.at<float>(j, i));
    }
  }
  return newMap;
}
  void MedianMapFilter::SetKernel(int KernelSize) {
    KernelSize_ = KernelSize;
  }
}  // namespace core
