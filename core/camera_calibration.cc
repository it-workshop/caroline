// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author: Kukleva Anna <Kuklevaanna@gmail.com>

#include "core/camera_calibration.h"

#include <vector>
#include "core/cameras.h"

namespace core {

Cameras CameraCalibration::calibrate(int nx, int ny, float square_size) {
  int n = nx * ny;
  int count_of_pairs;
  cv::Matx33d K1;
  cv::Matx33d K2;
  cv::Matx33d R;
  cv::Matx31d T;
  cv::Matx33d E;
  cv::Matx33d F;
  cv::Mat D_1;
  cv::Mat D_2;
  std::vector<double> D1(5, 0);
  std::vector<double> D2(5, 0);
  cv::Matx33d first;
  cv::Matx33d second;

  // HARVEST CHESSBOARD 3D OBJECT POINT LIST:
  count_of_pairs = VPoints1.size();

  std::vector<std::vector<cv::Point3f>> VobjectPoints(count_of_pairs);

  for (int i = 0; i < count_of_pairs; i++) {
    VobjectPoints[i] = object_points_;
  }

  CalibrationOneCamera(kLeft, nx, ny, square_size, &first, &D1);
  CalibrationOneCamera(kRight, nx, ny, square_size, &second, &D2);

  D_1 = cv::Mat(D1);
  D_2 = cv::Mat(D2);

  // StereoCalibrate
  cv::stereoCalibrate(VobjectPoints, VPoints1,
      VPoints2, first, D1, second, D2,
      image_size_, R, T, E, F);

  cv::Matx34d P1(1, 0, 0, 0,
                  0, 1, 0, 0,
                  0, 0, 1, 0);
  cv::Matx34d P2(R(0, 0), R(0, 1), R(0, 2), T(0, 0),
                 R(1, 0), R(1, 1), R(1, 2), T(1, 0),
                 R(2, 0), R(2, 1), R(2, 2), T(0, 0));

  Cameras cameras_par;
  cameras_par.set_K1(first);
  cameras_par.set_K2(second);
  cameras_par.set_P1(P1);
  cameras_par.set_P2(P2);
  cameras_par.set_R(R);
  cameras_par.set_T(T);
  cameras_par.set_D1(D_1);
  cameras_par.set_D2(D_2);

  return cameras_par;
}

void CameraCalibration::addImagePair(const cv::Mat &image1,
                                     const cv::Mat &image2, int nx, int ny) {
  image_size_ = image1.size();

  // harvest points of corners for one image
  std::vector<cv::Point2f> temp1(nx * ny);
  std::vector<cv::Point2f> temp2(nx * ny);

  int result1 = 0;
  int result2 = 0;

  // Finds the positions of
  // internal corners of the chessboard
  result1 = cv::findChessboardCorners(
        image1, cv::Size(nx, ny), temp1);
  result2 = cv::findChessboardCorners(
        image2, cv::Size(nx, ny), temp2);

  if (!VPoints1.size()) {
    image_size_ = image1.size();
  }

  if (result1 && result2) {
    // Refines the corner locations
    cv::cornerSubPix(image1, temp1, cv::Size(5, 5),
              cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::EPS +
              cv::TermCriteria::COUNT, 30, 0.01));
    cv::cornerSubPix(image2, temp2, cv::Size(11, 11),
              cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::EPS +
              cv::TermCriteria::COUNT, 30, 0.01));
    VPoints1.push_back(temp1);
    VPoints2.push_back(temp2);
  }
}

void CameraCalibration::HarvestChessboardIdealPointList(
    int nx, int ny, float square_size) {
  object_points_.resize(nx * ny);

  for (int i = 0; i < ny; i++) {
    for (int j = 0; j < nx; j++) {
      object_points_[i*nx+j] = cv::Point3f(i*square_size, j*square_size, 0);
    }
  }
}

void CameraCalibration::CalibrationOneCamera(
    int CameraOrientation, int nx, int ny,
    float square_size, cv::Matx33d* K, std::vector<double>* D) {
  std::vector<std::vector<cv::Point2f>> VPoints;
  if (CameraOrientation == kLeft) {
    VPoints = VPoints1;
  } else {
    VPoints = VPoints2;
  }

  int result = 0;
  int n = nx * ny;
  int counter;
  std::vector<cv::Point2f> temp(n);
  std::vector<cv::Mat> vrvec;
  std::vector<cv::Mat> tvec;

  counter = VPoints.size();
  std::vector<std::vector<cv::Point3f>> VobjectPoints(counter);

  for (int i = 0; i < counter; i++) {
    VobjectPoints[i] = object_points_;
  }

  cv::setIdentity(*K);

#if CV_MAJOR > 2 || (CV_MAJOR == 2 && CV_MINOR >= 4)
  cv::calibrateCamera(
        VobjectPoints, VPoints, image_size_, *K, *D, vrvec, tvec, 0,
        cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT,
                         30, 0.01));
#else
  cv::calibrateCamera(
        VobjectPoints, VPoints, image_size_, *K, *D, vrvec, tvec, 0);
#endif
}

}  //  namespace core
