// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author: Kukleva Anna <Kuklevaanna@gmail.com>

#include <memory>
#include <string>
#include "gtest/gtest.h"
#include "core/camera_calibration.h"
#include "core/cameras.h"

#if CV_VERSION_MAJOR > 2 || (CV_VERSION_MAJOR == 2 && CV_VERSION_MINOR >= 4)
#define CV_IMREAD_GRAYSCALE cv::IMREAD_GRAYSCALE
#else
#define CV_IMREAD_GRAYSCALE 0
#endif

#if CV_VERSION_MAJOR > 2 || (CV_VERSION_MAJOR == 2 && CV_VERSION_MINOR >= 4)
#define MAYBE_Test1 Test1
#else
#define MAYBE_Test1 DISABLED_Test1
#endif
TEST(CamerasCalibrateTest, MAYBE_Test1) {
  core::Cameras cam;
  core::CameraCalibration calib;

  double epsilon = 1.002;
  std::string str = "0";

  for ( int i = 0; i < 4; i++ ) {
    cv::Mat left_im = cv::imread("test_images/camera_calibration/left0" +
      str +".ppm", CV_IMREAD_GRAYSCALE);

    cv::Mat right_im = cv::imread("test_images/camera_calibration/right0" +
      str +".ppm", CV_IMREAD_GRAYSCALE);
    str = std::to_string(i + 1);
    calib.addImagePair(left_im, right_im, 9, 6);
  }

  calib.HarvestChessboardIdealPointList(9, 6, 28);
  cam = calib.calibrate(9, 6, 2.8);

  EXPECT_LE((fabs(cam.P2()(0, 3) - (-79.2473))), epsilon);
  EXPECT_LE((fabs(cam.P2()(1, 3) - (6.77501))), epsilon);
  EXPECT_LE((fabs(cam.P2()(2, 3) - (-79.2473))), epsilon);
}

#if CV_VERSION_MAJOR > 2 || (CV_VERSION_MAJOR == 2 && CV_VERSION_MINOR >= 4)
#define MAYBE_TestForOneCam TestForOneCam
#else
#define MAYBE_TestForOneCam DISABLED_TestForOneCam
#endif
TEST(CamerasCalibrateTest, MAYBE_TestForOneCam) {
  core::CameraCalibration calib;

  cv::Matx33d intr_mat;
  double epsilon = 2.002;
  std::string str = "0";
  std::vector<double> D;

  for ( int i = 0; i < 4; i++ ) {
    cv::Mat left_im = cv::imread("test_images/camera_calibration/left0" +
      str + ".ppm", CV_IMREAD_GRAYSCALE);
    cv::Mat right_im = cv::imread("test_images/camera_calibration/right0" +
      str + ".ppm", CV_IMREAD_GRAYSCALE);
    str = std::to_string(i + 1);
    calib.addImagePair(left_im, right_im, 9, 6);
  }

  calib.HarvestChessboardIdealPointList(9, 6, 28);
  calib.CalibrationOneCamera(
        core::CameraCalibration::kLeft, 9, 6, 28, &intr_mat, &D);

  EXPECT_LE((fabs(intr_mat(0, 2) - 318.873)), epsilon);
  EXPECT_LE((fabs(intr_mat(1, 2) - 239.406)), epsilon);

  calib.CalibrationOneCamera(
        core::CameraCalibration::kRight, 9, 6, 28, &intr_mat, &D);

  EXPECT_LE((fabs(intr_mat(0, 2) - 319.581)), epsilon);
  EXPECT_LE((fabs(intr_mat(1, 2) - 239.706)), epsilon);
}
