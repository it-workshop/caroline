// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_TRIANGULATION_EIGEN_H_
#define CORE_TRIANGULATION_EIGEN_H_

#include "core/triangulation.h"

namespace core {

// For more information about the algorithm, see
// www.morethantechnical.com/2012/01/04/simple-triangulation-with-opencv-from-harley-zisserman-w-code  /NOLINT
class TriangulationEigen : public Triangulation {
 public:
  TriangulationEigen();

  ~TriangulationEigen() {}

  void SetCameraMatrices(const Cameras& cameras) override;

  void SetIterations(int iter) { iterations_ = iter; }
  void SetEpsilon(double eps) { epsilon_ = eps; }

  int Iterations() const { return iterations_; }
  double Epsilon() const { return epsilon_; }

  cv::Point3d Triangulate(const cv::Point2d &x1,
      const cv::Point2d &x2) const override;
  double TriangulateDepth(const cv::Point2d &x1,
      const cv::Point2d &x2) const override;

 protected:
  cv::Mat_<double> LinearTriangulation(const cv::Point3d &x1,
                                       const cv::Point3d x2) const;
  cv::Mat_<double> IterativeTriangulation(const cv::Point3d& x1,
                                          const cv::Point3d& x2) const;

 private:
  cv::Matx33d k_1_inv_;
  cv::Matx33d k_2_inv_;

  int iterations_;
  double epsilon_;
};

}  // namespace core

#endif  // CORE_TRIANGULATION_EIGEN_H_
