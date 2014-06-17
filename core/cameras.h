// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_CAMERAS_H_
#define CORE_CAMERAS_H_

#include "opencv2/opencv.hpp"

namespace core {

class Cameras {
 public:
  Cameras();

  void set_K1(const cv::Matx33d& K1) { K1_ = K1; }
  void set_K2(const cv::Matx33d& K2) { K2_ = K2; }
  void set_P1(const cv::Matx34d& P1) { P1_ = P1; }
  void set_P2(const cv::Matx34d& P2) { P2_ = P2; }

  cv::Matx33d K1() const { return K1_; }
  cv::Matx33d K2() const { return K2_; }
  cv::Matx34d P1() const { return P1_; }
  cv::Matx34d P2() const { return P2_; }

 private:
  // camera one matrix
  cv::Matx33d K1_;
  // camera two matrix
  cv::Matx33d K2_;
  // projective matrix one
  cv::Matx34d P1_;
  // projective matrix two
  cv::Matx34d P2_;
};

}  // namespace core

#endif  // CORE_CAMERAS_H_
