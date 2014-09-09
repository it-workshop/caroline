// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "core/metrics.h"
#include "core/stat_utils.h"

#include "opencv2/core.hpp"

namespace core {

namespace stat {

double SimpleDist::compute(const cv::Mat &m1, const cv::Mat &m2) {
  cv::Mat temp1;
  cv::Mat temp2;
  m1.convertTo(temp1, CV_32F);
  m2.convertTo(temp2, CV_32F);
  cv::multiply(temp1, temp1, temp1);
  cv::multiply(temp2, temp2, temp2);

  cv::Mat dst;
  cv::absdiff(temp1, temp2, dst);
  cv::sqrt(dst, dst);
  return cv::sum(dst)[0];
}

double RSquare::compute(const cv::Mat &m1, const cv::Mat &m2) {
  cv::Mat temp1;
  cv::Mat temp2;
  m1.convertTo(temp1, CV_32F);
  m2.convertTo(temp2, CV_32F);

  cv::Scalar sd;
  cv::Scalar sdDiff;
  cv::meanStdDev(m1, cv::Scalar(), sd);
  cv::meanStdDev(m1 - m2, cv::Scalar(), sdDiff);

  return (1 - sdDiff[0] / sd[0]);
}

}  // namespace stat

}  // namespace core
