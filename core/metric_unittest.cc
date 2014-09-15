// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include <memory>
#include <string>

#include "base/path.h"
#include "base/path_service.h"
#include "core/metric_factory.h"
#include "gtest/gtest.h"
#include "opencv2/highgui.hpp"

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

#if CV_VERSION_MAJOR > 2 || (CV_VERSION_MAJOR == 2 && CV_VERSION_MINOR >= 4)
#define MAYBE_SimpleDist SimpleDist
#else
#define MAYBE_SimpleDist DISABLED_SimpleDist
#endif
TEST(MetricsTest, MAYBE_SimpleDist) {
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

  std::unique_ptr<core::stat::Metric> obj =
      core::stat::MetricFactory::Create("simple_dist");
  EXPECT_LE(obj->compute(im_true, im_comp) / (im_true.rows * im_true.cols), 20);
}

#if CV_VERSION_MAJOR > 2 || (CV_VERSION_MAJOR == 2 && CV_VERSION_MINOR >= 4)
#define MAYBE_RSquare RSquare
#else
#define MAYBE_RSquare DISABLED_RSquare
#endif
TEST(MetricsTest, MAYBE_RSquare) {
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

  std::unique_ptr<core::stat::Metric> obj =
      core::stat::MetricFactory::Create("RSquare");
  EXPECT_GE(obj->compute(im_true, im_comp), 0.9);
}
