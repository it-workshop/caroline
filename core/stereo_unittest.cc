// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include <memory>

#include "gtest/gtest.h"
#include "opencv2/opencv.hpp"

#include "core/stereo_depth_map.h"

TEST(StereoTest, CreateAndSaveMesh) {
  core::StereoDepthMap stereo;

  cv::Mat map = stereo.BuildMap("/home/rayman/Downloads/ALL-2views/Bowling2/view1.png", "/home/rayman/Downloads/ALL-2views/Bowling2/view5.png");

  cv::imshow( "Display window", map);
  cv::waitKey(0);

  EXPECT_NE(map.cols, 0);
  EXPECT_NE(map.rows, 0);
}
