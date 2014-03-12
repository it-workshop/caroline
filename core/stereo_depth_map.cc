// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "stereo_depth_map.h"

#include "gtest/gtest.h"

namespace core {

cv::Mat StereoDepthMap::BuildMap(const std::string &leftImage, const std::string &rightImage) {
  cv::StereoSGBM stereo(0, 32, 9);

  cv::Mat leftIm = cv::imread(leftImage, CV_LOAD_IMAGE_GRAYSCALE);
  cv::Mat rightIm = cv::imread(rightImage, CV_LOAD_IMAGE_GRAYSCALE);

  if ((!leftIm.data) || (!rightIm.data)) {
    return cv::Mat(0,0,CV_8UC1);
  }

  if (leftIm.rows != rightIm.rows)
      return cv::Mat(0,0,CV_8UC1);

  if (leftIm.cols != rightIm.cols)
    return cv::Mat(0,0,CV_8UC1);

  int h = leftIm.rows;
  int w = leftIm.cols;

  cv::Mat disp = cv::Mat(h, w, CV_16S);
  cv::Mat disp8U = cv::Mat(h, w, CV_8UC1 );

  stereo(leftIm, rightIm, disp);

  double minVal; double maxVal;
  minMaxLoc(disp, &minVal, &maxVal );

  disp.convertTo(disp8U, CV_8UC1, 255/(maxVal - minVal));

  return disp8U;
}

}  // namespace core
