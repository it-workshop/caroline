// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author:


#include <vector>
#include "core/camera_calibration.h"
#include "core/cameras.h"

namespace core {

Cameras CameraCalibration::calibrate(int nx, int ny, float square_size) {
  int n = nx*ny, len;
  int count_of_pairs;
  cv::Matx33d K1_;
  cv::Matx33d K2_;
  cv::Matx33d R_;
  cv::Matx31d T_;
  cv::Matx33d E_;
  cv::Matx33d F_;
  std::vector<double> D1(5, 0);
  std::vector<double> D2(5, 0);


  cv::Size imageSize = {0, 0};
  imageSize = left_images_.at(0).size();
  len = left_images_.size();


  // HARVEST CHESSBOARD 3D OBJECT POINT LIST:
  count_of_pairs = VPoints1.size();


  std::vector<std::vector<cv::Point3f>> VobjectPoints(count_of_pairs);


  for (int i = 0; i < count_of_pairs; i++)
    VobjectPoints[i] = object_points_;


  first_ = CalebrationOneCamera(kLeft, nx, ny, square_size);
  D1 = D;
  second_ = CalebrationOneCamera(kRight, nx, ny, square_size);
  D2 = D;

  // StereoCalibrate
  cv::stereoCalibrate(VobjectPoints, VPoints1,
      VPoints2, first_, D1, second_, D2,
      imageSize, R_, T_, E_, F_);


  cv::Matx34d P1_(1, 0, 0, 0,
                  0, 1, 0, 0,
                  0, 0, 1, 0);
  cv::Matx34d P2_(R_(0, 0), R_(0, 1), R_(0, 2), T_(0, 0),
                  R_(1, 0), R_(1, 1), R_(1, 2), T_(1, 0),
                  R_(2, 0), R_(2, 1), R_(2, 2), T_(0, 0));

  Cameras cameras_par;
  cameras_par.set_K1(first_);
  cameras_par.set_K2(second_);
  cameras_par.set_P1(P1_);
  cameras_par.set_P2(P2_);

  return cameras_par;
}

  void CameraCalibration::addImagePair(const cv::Mat &image1,
  const cv::Mat &image2, int nx, int ny) {
    left_images_.push_back(image1);
    right_images_.push_back(image2);

    // harvest points of corners for one image
    std::vector<cv::Point2f> temp1(nx*ny);
    std::vector<cv::Point2f> temp2(nx*ny);

    int len, result1 = 0, result2 = 0;

      cv::Mat img1 = left_images_.back();
      cv::Mat img2 = right_images_.back();
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

  void CameraCalibration::HarvestChessboardIdealPointList
                      (int nx, int ny, float square_size) {
    object_points_.resize(nx*ny);

    for (int i = 0; i < ny; i++)
      for (int j = 0; j < nx; j++)
        object_points_[i*nx+j] = cv::Point3f(i*square_size, j*square_size, 0);
  }

  cv::Matx33d CameraCalibration::CalebrationOneCamera(int CameraOrientation,
                                          int nx, int ny, float square_size) {
    std::vector<std::vector<cv::Point2f>> VPoints;
    std::vector<cv::Mat> images;
    if (CameraOrientation == kLeft) {
      images = left_images_;
      VPoints = VPoints1;
    } else {
      images = right_images_;
      VPoints = VPoints2;
    }


    int result= 0, n = nx*ny, counter, length;
    std::vector<cv::Point2f> temp(n);
    cv::Size imageSize = {0, 0};
    imageSize = images.at(0).size();
    cv::Matx33d K;
    std::vector<cv::Mat> vrvec;
    std::vector<cv::Mat> tvec;

    length = images.size();

    counter = VPoints.size();
    std::vector<std::vector<cv::Point3f>> VobjectPoints(counter);

    for (int i = 0; i < counter; i++)
      VobjectPoints[i] = object_points_;

    cv::setIdentity(K);

    cv::calibrateCamera(VobjectPoints, VPoints, imageSize, K, D, vrvec, tvec, 0,
    cv::TermCriteria(cv::TermCriteria::EPS +cv::TermCriteria::COUNT, 30, 0.01));


    return K;
  }

}  //  namespace core
