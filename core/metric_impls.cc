// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "core/metric_impls.h"

#include "base/logging.h"
#include "opencv2/core/core.hpp"

namespace core {

namespace stat {

long double Mean::compute(const std::vector<cv::Mat> &src) {
  if (src.size() != 1) {
    LOG(INFO) << "Compute metric error. Need one matrices to compute.";
  }

  cv::Scalar sd;
  cv::Scalar mean;
  cv::meanStdDev(src.at(0), mean, sd);

  long double temp = mean[0];

  LOG(INFO) << "Compute metric = " << temp;
  return temp;
}

long double SimpleDist::compute(const std::vector<cv::Mat> &src) {
  if (src.size() != 2) {
    LOG(INFO) << "Compute metric error. Need two matrices to compute.";
  }
  cv::Mat m1 = src.at(0);
  cv::Mat m2 = src.at(1);

  cv::Mat temp1;
  cv::Mat temp2;
  m1.convertTo(temp1, CV_32F);
  m2.convertTo(temp2, CV_32F);
  cv::multiply(temp1, temp1, temp1);
  cv::multiply(temp2, temp2, temp2);

  cv::Mat dst;
  cv::absdiff(temp1, temp2, dst);
  cv::sqrt(dst, dst);
  long double temp = cv::sum(dst)[0];

  LOG(INFO) << "computed metric = " << temp;
  return temp;
}

long double RSquare::compute(const std::vector<cv::Mat> &src) {
  if (src.size() != 2) {
    LOG(INFO) << "Compute metric error. Need two matrices to compute.";
  }
  cv::Mat m1 = src.at(0);
  cv::Mat m2 = src.at(1);

  cv::Mat temp1;
  cv::Mat temp2;
  m1.convertTo(temp1, CV_32F);
  m2.convertTo(temp2, CV_32F);

  cv::Scalar sd;
  cv::Scalar mean;
  cv::Scalar sdDiff;
  cv::Scalar meanDiff;
  cv::meanStdDev(m1, mean, sd);
  cv::meanStdDev(m1 - m2, meanDiff, sdDiff);
  long double temp = (1 - sdDiff[0] / sd[0]);

  LOG(INFO) << "Compute metric = " << temp;
  return temp;
}

}  // namespace stat

}  // namespace core
