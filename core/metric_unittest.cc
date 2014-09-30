// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include <memory>
#include <string>
#include <vector>

#include "base/path.h"
#include "base/path_service.h"
#include "core/metric_factory.h"
#include "gtest/gtest.h"
#include "opencv2/opencv.hpp"

namespace {

const char kTestImagesDirectory[] = "test_images";
const char kMetricsTestImagesDirectory[] = "metrics";
const char kDisparity[] = "disparity.png";
const char kDispWithNoise[] = "simplenoise.png";

}  // namespace

#if CV_VERSION_MAJOR > 2 || (CV_VERSION_MAJOR == 2 && CV_VERSION_MINOR >= 4)
#define CV_IMREAD_GRAYSCALE cv::IMREAD_GRAYSCALE
#else
#define CV_IMREAD_GRAYSCALE 0
#endif

TEST(MetricsTest, SimpleDist) {
  base::Path image_directory =
      base::PathService::GetInstance()->executable_path().
      ParentDir().ParentDir().
      Join(kTestImagesDirectory).Join(kMetricsTestImagesDirectory);

  const base::Path& true_disp_path = image_directory.Join(kDisparity);
  const base::Path& noise_disp_path = image_directory.Join(kDispWithNoise);

  cv::Mat im_true = cv::imread(true_disp_path.spec(),
                               CV_IMREAD_GRAYSCALE);
  cv::Mat im_comp = cv::imread(noise_disp_path.spec(),
                               CV_IMREAD_GRAYSCALE);

  EXPECT_EQ(im_true.size, im_comp.size);

  std::vector<cv::Mat> src;
  src.push_back(im_true);
  src.push_back(im_comp);

  std::unique_ptr<core::stat::Metric> obj =
      core::stat::MetricFactory::Create("simple_dist");

  EXPECT_LE(obj->compute(src) / (im_true.rows * im_true.cols), 20);
}

TEST(MetricsTest, RSquare) {
  base::Path image_directory =
      base::PathService::GetInstance()->executable_path().
      ParentDir().ParentDir().
      Join(kTestImagesDirectory).Join(kMetricsTestImagesDirectory);

  const base::Path& true_disp_path = image_directory.Join(kDisparity);
  const base::Path& noise_disp_path = image_directory.Join(kDispWithNoise);

  cv::Mat im_true = cv::imread(true_disp_path.spec(),
                               CV_IMREAD_GRAYSCALE);
  cv::Mat im_comp = cv::imread(noise_disp_path.spec(),
                               CV_IMREAD_GRAYSCALE);

  EXPECT_EQ(im_true.size, im_comp.size);

  std::vector<cv::Mat> src;
  src.push_back(im_true);
  src.push_back(im_comp);

  std::unique_ptr<core::stat::Metric> obj =
      core::stat::MetricFactory::Create("RSquare");
  EXPECT_GE(obj->compute(src), 0.9);
}
