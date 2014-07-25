// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_CAMERAS_H_
#define CORE_CAMERAS_H_

#include <memory>
#include "opencv2/core/mat.hpp"

namespace core {

class Quaternion;

class Cameras {
 public:
  Cameras();

  virtual ~Cameras() {}

  static cv::Matx33d CameraMatrix(int dpm, double focus_length, int w, int h);
  static cv::Matx34d ProjectiveMatrix(const Quaternion& quat,
                                      const cv::Point3d& pos);

  void set_K1(const cv::Matx33d& K1) { K1_ = K1; }
  void set_K2(const cv::Matx33d& K2) { K2_ = K2; }
  void set_P1(const cv::Matx34d& P1) { P1_ = P1; }
  void set_P2(const cv::Matx34d& P2) { P2_ = P2; }

  cv::Matx33d K1() const { return K1_; }
  cv::Matx33d K2() const { return K2_; }
  cv::Matx34d P1() const { return P1_; }
  cv::Matx34d P2() const { return P2_; }

  cv::Mat D1() const { return D1_; }
  cv::Mat D2() const { return D2_; }
  cv::Mat P() const { return P_; }
  cv::Mat T() const { return T_; }

 private:
  // camera one matrix
  cv::Matx33d K1_;
  // camera two matrix
  cv::Matx33d K2_;
  // projective matrix one
  cv::Matx34d P1_;
  // projective matrix two
  cv::Matx34d P2_;
  // rotation matrix from camera one to camera 2
  cv::Matx33d P_;
  // translation vector
  cv::Mat T_;

  // cameras distortion matrix
  cv::Mat D1_;
  cv::Mat D2_;
};

}  // namespace core

#endif  // CORE_CAMERAS_H_
