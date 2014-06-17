// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include <memory>

#include "gtest/gtest.h"

#include "core/cameras.h"
#include "core/quaternion.h"

#include "iostream"

TEST(CamerasPropertiesTest, InitTest) {
  core::Cameras cams;

  cams.set_K1(cv::Matx33d(1, 0, 0,
                          0, 1, 0,
                          0, 0, 1));

  ASSERT_TRUE(cams.K1()(0, 0) == 1);
  ASSERT_TRUE(cams.K1()(1, 2) == 0);
  ASSERT_TRUE(cams.K1()(2, 2) == 1);
}

TEST(CamerasPropertiesTest, CameraMatrixCompute) {
  int w = 512;
  int h = 256;

  int dpm = 1000;
  double focus_length = 0.05;

  cv::Matx33d K = core::Cameras::CameraMatrix(dpm, focus_length, w, h);

  ASSERT_TRUE(K(0, 0) == 50);
  ASSERT_TRUE(K(1, 1) == 50);
  ASSERT_TRUE(K(2, 2) == 1);
  ASSERT_TRUE(K(0, 1) == 0);
  ASSERT_TRUE(K(0, 2) = 256);
  ASSERT_TRUE(K(1, 2) = 128);
}

TEST(CamerasPropertiesTest, ProjectiveMatrixCompute) {
  core::Quaternion quat(3.1415, 1, 0, 0);

  double epsilon = 0.002;

  cv::Point3d pos;
  pos.x = 10;
  pos.y = 12;
  pos.z = 0;

  cv::Matx34d P = core::Cameras::ProjectiveMatrix(quat, pos);

  ASSERT_TRUE(fabs(P(0, 0) - 1) < epsilon);
  ASSERT_TRUE(fabs(P(1, 1) - (-1)) < epsilon);
  ASSERT_TRUE(fabs(P(2, 2) - (-1)) < epsilon);
  ASSERT_TRUE(P(0, 3) == 10);
  ASSERT_TRUE(P(1, 0) < epsilon);
}
