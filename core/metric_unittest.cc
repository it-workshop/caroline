// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "gtest/gtest.h"

#include "opencv2/highgui.hpp"

#include "core/metric_factory.h"

TEST(MetricsTest, SimpleDist) {
  cv::Mat im_true = cv::imread("../test_images/metrics/disparity.png",
                               cv::IMREAD_GRAYSCALE);
  cv::Mat im_comp = cv::imread("../test_images/metrics/simplenoise.png",
                               cv::IMREAD_GRAYSCALE);

  EXPECT_EQ(im_true.size, im_comp.size);

  std::unique_ptr<core::stat::Metric> obj =
      core::stat::MetricFactory::Create("simple_dist");
  EXPECT_LE(obj->compute(im_true, im_comp) / (im_true.rows * im_true.cols), 20);
}

TEST(MetricsTest, RSquare) {
  cv::Mat im_true = cv::imread("../test_images/metrics/disparity.png",
                               cv::IMREAD_GRAYSCALE);
  cv::Mat im_comp = cv::imread("../test_images/metrics/simplenoise.png",
                               cv::IMREAD_GRAYSCALE);

  EXPECT_EQ(im_true.size, im_comp.size);

  std::unique_ptr<core::stat::Metric> obj =
      core::stat::MetricFactory::Create("RSquare");
  EXPECT_GE(obj->compute(im_true, im_comp), 0.9);
}
