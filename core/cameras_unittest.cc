// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include <memory>

#include "gtest/gtest.h"

#include "core/cameras.h"
#include "core/quaternion.h"

TEST(CamerasPropertiesTest, InitTest) {
  core::Cameras cams;

  cams.set_K1(cv::Matx33d(1, 0, 0,
                          0, 1, 0,
                          0, 0, 1));

  EXPECT_EQ(1, cams.K1()(0, 0));
  EXPECT_EQ(0, cams.K1()(1, 2));
  EXPECT_EQ(1, cams.K1()(2, 2));
}

TEST(CamerasPropertiesTest, CameraMatrixCompute) {
  int w = 512;
  int h = 256;

  int dpm = 1000;
  double focus_length = 0.05;

  cv::Matx33d K = core::Cameras::CameraMatrix(dpm, focus_length, w, h);

  EXPECT_EQ(50, K(0, 0));
  EXPECT_EQ(50, K(1, 1));
  EXPECT_EQ(1, K(2, 2));
  EXPECT_EQ(0, K(0, 1));
  EXPECT_EQ(256, K(0, 2));
  EXPECT_EQ(128, K(1, 2));
}

TEST(CamerasPropertiesTest, ProjectiveMatrixCompute) {
  core::Quaternion quat(3.1415, 1, 0, 0);

  double epsilon = 0.002;

  cv::Point3d pos;
  pos.x = 10;
  pos.y = 12;
  pos.z = 0;

  cv::Matx34d P = core::Cameras::ProjectiveMatrix(quat, pos);

  EXPECT_GT(epsilon, fabs(P(0, 0) - 1));
  EXPECT_GT(epsilon, fabs(P(1, 1) - (-1)));
  EXPECT_GT(epsilon, fabs(P(2, 2) - (-1)));
  EXPECT_EQ(10, P(0, 3));
  EXPECT_GT(epsilon, P(1, 0));
}

