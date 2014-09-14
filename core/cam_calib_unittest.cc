// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author: Kukleva Anna <Kuklevaanna@gmail.com>

#include <memory>
#include <string>

#include "base/path.h"
#include "base/path_service.h"
#include "core/camera_calibration.h"
#include "core/cameras.h"
#include "gtest/gtest.h"

namespace {

const char kTestImagesDirectory[] = "test_images";
const char kCameraCalibrationTestImagesDirectory[] = "camera_calibration";
const char kLeftImageBaseName[] = "left0";
const char kRightImageBaseName[] = "right0";
const char kImageExtension[] = ".ppm";

}  // namespace

#if CV_MAJOR > 2 || (CV_MAJOR == 2 && CV_MINOR >= 4)
#define CV_IMREAD_GRAYSCALE cv::IMREAD_GRAYSCALE
#else
#define CV_IMREAD_GRAYSCALE 0
#endif

#if CV_MAJOR > 2 || (CV_MAJOR == 2 && CV_MINOR >= 4)
#define MAYBE_Test1 Test1
#else
#define MAYBE_Test1 DISABLED_Test1
#endif
TEST(CamerasCalibrateTest, MAYBE_Test1) {
  core::Cameras cam;
  core::CameraCalibration calib;

  const double kEpsilon = 1.002;

  base::Path images_directory =
      base::PathService::GetInstance()->executable_path().
      ParentDir().ParentDir().
      Join("test_images").Join("camera_calibration");

  for (int i = 0; i < 4; i++) {
    const base::Path& left_image_path = images_directory.Join(
        kLeftImageBaseName + std::to_string(i) + kImageExtension);
    const base::Path& right_image_path = images_directory.Join(
        kRightImageBaseName + std::to_string(i) + kImageExtension);

    cv::Mat left_im = cv::imread(left_image_path.spec(), CV_IMREAD_GRAYSCALE);
    cv::Mat right_im = cv::imread(right_image_path.spec(),
        CV_IMREAD_GRAYSCALE);
    calib.addImagePair(left_im, right_im, 9, 6);
  }

  calib.HarvestChessboardIdealPointList(9, 6, 28);
  cam = calib.calibrate(9, 6, 2.8);

  EXPECT_LE((fabs(cam.P2()(0, 3) - (-79.2473))), kEpsilon);
  EXPECT_LE((fabs(cam.P2()(1, 3) - (6.77501))), kEpsilon);
  EXPECT_LE((fabs(cam.P2()(2, 3) - (-79.2473))), kEpsilon);
}

#if CV_MAJOR > 2 || (CV_MAJOR == 2 && CV_MINOR >= 4)
#define MAYBE_TestForOneCam TestForOneCam
#else
#define MAYBE_TestForOneCam DISABLED_TestForOneCam
#endif
TEST(CamerasCalibrateTest, MAYBE_TestForOneCam) {
  core::CameraCalibration calib;

  cv::Matx33d intr_mat;
  const double kEpsilon = 2.002;
  std::vector<double> D;

  base::Path images_directory =
      base::PathService::GetInstance()->executable_path().
      ParentDir().ParentDir().
      Join("test_images").Join("camera_calibration");

  for (int i = 0; i < 4; i++) {
    const base::Path& left_image_path = images_directory.Join(
        kLeftImageBaseName + std::to_string(i) + kImageExtension);
    const base::Path& right_image_path = images_directory.Join(
        kRightImageBaseName + std::to_string(i) + kImageExtension);

    cv::Mat left_im = cv::imread(left_image_path.spec(), CV_IMREAD_GRAYSCALE);
    cv::Mat right_im = cv::imread(right_image_path.spec(),
        CV_IMREAD_GRAYSCALE);
    calib.addImagePair(left_im, right_im, 9, 6);
  }

  calib.HarvestChessboardIdealPointList(9, 6, 28);
  calib.CalibrationOneCamera(
        core::CameraCalibration::kLeft, 9, 6, 28, &intr_mat, &D);

  EXPECT_LE((fabs(intr_mat(0, 2) - 318.873)), kEpsilon);
  EXPECT_LE((fabs(intr_mat(1, 2) - 239.406)), kEpsilon);

  calib.CalibrationOneCamera(
        core::CameraCalibration::kRight, 9, 6, 28, &intr_mat, &D);

  EXPECT_LE((fabs(intr_mat(0, 2) - 319.581)), kEpsilon);
  EXPECT_LE((fabs(intr_mat(1, 2) - 239.706)), kEpsilon);
}
