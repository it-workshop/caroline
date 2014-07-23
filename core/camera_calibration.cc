// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author:


#include <vector>
#include "core/camera_calibration.h"
#include "core/cameras.h"


namespace core {

Cameras CameraCalibration::calibrate(int nx, int ny, float square_size ) {
  int n = nx*ny, len, result1 = 0, result2 = 0, max_count_iter = 30;
  int count_of_pairs;
  float eps_acc = 0.01;
  cv::Matx33d K1_;
  cv::Matx33d K2_;
  cv::Matx33d R_;
  cv::Matx31d T_;
  cv::Matx33d E_;
  cv::Matx33d F_;
  std::vector<double> D1(5, 0);
  std::vector<double> D2(5, 0);
  std::vector<cv::Point2f> temp1(n);  // harvest points of corners for one image
  std::vector<cv::Point2f> temp2(n);
  std::vector<cv::Point3f> objectPoints;
  std::vector<std::vector<cv::Point2f>> VPoints1(0);
  std::vector<std::vector<cv::Point2f>> VPoints2(0);
  cv::Size imageSize = {0, 0};
  imageSize = left_images_.at(0).size();
  len = left_images_.size();

  for (int i = 0; i < len; i++) {
    cv::Mat img1 = left_images_.at(i);
    cv::Mat img2 = right_images_.at(i);
    result1 = cv::findChessboardCorners(img1,
    cv::Size(nx, ny), temp1);  // Finds the positions of
    result2 = cv::findChessboardCorners(img2,
    cv::Size(nx, ny), temp2);  // internal corners of the chessboard
    if (result1 && result2) {
      // Refines the corner locations
      cv::cornerSubPix(img1, temp1, cv::Size(5, 5),
      cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::EPS +
      cv::TermCriteria::COUNT, 30, 0.01));
      cv::cornerSubPix(img2, temp2, cv::Size(11, 11),
      cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::EPS +
      cv::TermCriteria::COUNT, 30, 0.01));
      VPoints1.push_back(temp1);
      VPoints2.push_back(temp2);
    }
  }

  // HARVEST CHESSBOARD 3D OBJECT POINT LIST:
  count_of_pairs = VPoints1.size();
  objectPoints.resize(n);
  std::vector<std::vector<cv::Point3f>> VObjectPoints(count_of_pairs);

  for (int i = 0; i < ny; i++) {
    for (int j = 0; j < nx; j++) {
      objectPoints[i*nx+j] = cv::Point3f(i*square_size, j*square_size, 0);
    }
  }

  for (int i = 0; i < count_of_pairs; i++)
    VObjectPoints[i] = objectPoints;

  cv::setIdentity(K1_);
  cv::setIdentity(K2_);

  // StereoCalibrate
  cv::stereoCalibrate(VObjectPoints, VPoints1,
      VPoints2, K1_, D1, K2_, D2,
      imageSize, R_, T_, E_, F_, cv::CALIB_USE_INTRINSIC_GUESS);
  cv::Matx34d P1_(1, 0, 0, 0,
                  0, 1, 0, 0,
                  0, 0, 1, 0);
  cv::Matx34d P2_(R_(0, 0), R_(0, 1), R_(0, 2), T_(0, 0),
                  R_(1, 0), R_(1, 1), R_(1, 2), T_(1, 0),
                  R_(2, 0), R_(2, 1), R_(2, 2), T_(0, 0));

  Cameras cameras_par;
  cameras_par.set_K1(K1_);
  cameras_par.set_K2(K2_);
  cameras_par.set_P1(P1_);
  cameras_par.set_P2(P2_);

  return cameras_par;
}

  void CameraCalibration::addImagePair(const cv::Mat &image1,
  const cv::Mat &image2) {
  left_images_.push_back(image1);
  right_images_.push_back(image2);
  }
}  //  namespace core
