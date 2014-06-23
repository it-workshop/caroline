// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_TRIANGULATION_H_
#define CORE_TRIANGULATION_H_

#include "opencv2/opencv.hpp"

#include "core/cameras.h"

namespace core {

// For more information about the algorithm, see
// www.morethantechnical.com/2012/01/04/simple-triangulation-with-opencv-from-harley-zisserman-w-code  /NOLINT
class Triangulation {
 public:
  Triangulation();

  ~Triangulation() {}

  void SetCameraMatrices(const cv::Matx33d& K1, const cv::Matx33d& K2,
                         const cv::Matx34d& P1, const cv::Matx34d& P2);


  void SetIterations(int iter) { iterations_ = iter; }
  void SetEpsilon(double eps) { epsilon_ = eps; }

  int Iterations() const { return iterations_; }
  double Epsilon() const { return epsilon_; }

  void SetCameraMatrices(const Cameras& cameras);

  cv::Point3d Triangulate(const cv::Point2d &x1, const cv::Point2d& x2) const;
  double TriangulateDepth(const cv::Point2d &x1, const cv::Point2d &x2) const;

 protected:
  cv::Mat_<double> LinearTriangulation(const cv::Point3d &x1,
                                       const cv::Point3d x2) const;
  cv::Mat_<double> IterativeTriangulation(const cv::Point3d& x1,
                                          const cv::Point3d& x2) const;

 private:
  int iterations_;
  double epsilon_;
  // camera one inverted matrix
  cv::Matx33d k_1_inv_;
  // camera two inverted matrix
  cv::Matx33d k_2_inv_;
  // projective matrix one
  cv::Matx34d p_1_;
  // projective matrix two
  cv::Matx34d p_2_;
};

}  // namespace core

#endif  // CORE_TRIANGULATION_H_
