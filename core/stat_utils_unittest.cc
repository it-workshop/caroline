// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "gtest/gtest.h"

#include "core/stat_utils.h"

TEST(StatUtilsTest, Reduce) {
  cv::Mat src(3, 3, CV_64F, cv::Scalar(2.));

  double sum = core::stat::reduce<double>(
        src.begin<double>(), src.end<double>(),
        [](double l, double r){ return l + r; });
  double mul = core::stat::reduce<double>(
        src.begin<double>(), src.end<double>(),
        [](double l, double r){ return l * r; });

  EXPECT_EQ(18, sum);
  EXPECT_EQ(512, mul);
}

TEST(StatUtilsTest, Sapply) {
  cv::Mat src(3, 3, CV_64F, cv::Scalar(0.));
  cv::Mat dst = src.clone();

  core::stat::sapply<double>(src, &dst, [](double x){ return x + 2; });

  EXPECT_EQ(0, src.at<double>(0, 0));
  EXPECT_EQ(0, src.at<double>(2, 2));
  EXPECT_EQ(2, dst.at<double>(0, 0));
  EXPECT_EQ(2, dst.at<double>(2, 2));
}
