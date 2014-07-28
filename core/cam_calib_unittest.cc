// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Kukleva Anna <Kuklevaanna@gmail.com>

#include <memory>
#include <string>
#include "gtest/gtest.h"
#include "core/camera_calibration.h"
#include "core/cameras.h"

TEST(CamerasCalibrateTest, Test1) {
  core::Cameras cam;
  core::CameraCalibration calib;

  double epsilon = 1.002;
  std::string str = "0";

  for ( int i = 0; i < 4; i++ ) {
    cv::Mat left_im = cv::imread("test_images/left0" +
      str +".ppm", cv::IMREAD_GRAYSCALE);

    cv::Mat right_im = cv::imread("test_images/right0" +
      str +".ppm", cv::IMREAD_GRAYSCALE);
    str = std::to_string(i + 1);
    calib.addImagePair(left_im, right_im, 9, 6);
  }

  calib.HarvestChessboardIdealPointList(9, 6, 2.8);
  cam = calib.calibrate(9, 6, 2.8);

  ASSERT_TRUE(fabs(cam.P2()(0, 3) - (-8.7705)) < epsilon);
  ASSERT_TRUE(fabs(cam.P2()(1, 3) - (-0.0513309)) < epsilon);
  ASSERT_TRUE(fabs(cam.P2()(2, 3) - (-8.7705)) < epsilon);
}

TEST(CamerasCalibrateTest, TestForOneCam) {
  core::CameraCalibration calib;

  cv::Matx33d intr_mat;
  double epsilon = 2.002;
  std::string str = "0";

  for ( int i = 0; i < 4; i++ ) {
    cv::Mat left_im = cv::imread("test_images/left0" +
      str + ".ppm", cv::IMREAD_GRAYSCALE);
    cv::Mat right_im = cv::imread("test_images/right0" +
      str + ".ppm", cv::IMREAD_GRAYSCALE);
    str = std::to_string(i + 1);
    calib.addImagePair(left_im, right_im, 9, 6);
  }

  calib.HarvestChessboardIdealPointList(9, 6, 2.8);
  intr_mat = calib.CalebrationOneCamera(
        core::CameraCalibration::kLeft, 9, 6, 2.8);

  calib.set_first(intr_mat);

  ASSERT_TRUE(fabs(intr_mat(0, 2) - 318.873) < epsilon);
  ASSERT_TRUE(fabs(intr_mat(1, 2) - 239.406) < epsilon);

  intr_mat = calib.CalebrationOneCamera(
        core::CameraCalibration::kRight, 9, 6, 2.8);
  calib.set_second(intr_mat);

  ASSERT_TRUE(fabs(intr_mat(0, 2) - 319.581) < epsilon);
  ASSERT_TRUE(fabs(intr_mat(1, 2) - 239.706) < epsilon);
}
