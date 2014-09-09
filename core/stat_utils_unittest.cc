// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "gtest/gtest.h"

#include "core/stat_utils.h"

TEST(StatUtilsTest, Reduce) {
  cv::Mat src(3, 3, CV_64F, cv::Scalar(2.));

  double sum = core::stat::reduce<double>(
        src, 0, [](double l, double r){ return l + r; });
  double mul = core::stat::reduce<double>(
        src, 1, [](double l, double r){ return l * r; });

  EXPECT_EQ(sum, 18);
  EXPECT_EQ(mul, 512);
}

TEST(StatUtilsTest, Sapply) {
  cv::Mat src(3, 3, CV_64F, cv::Scalar(0.));
  cv::Mat dst = src.clone();

  core::stat::sapply<double>(src, &dst, [](double x){ return x + 2; });

  EXPECT_EQ(src.at<double>(0, 0), 0);
  EXPECT_EQ(src.at<double>(2, 2), 0);
  EXPECT_EQ(dst.at<double>(0, 0), 2);
  EXPECT_EQ(dst.at<double>(2, 2), 2);
}
