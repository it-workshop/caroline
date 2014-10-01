// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_TRIANGULATION_EIGEN_H_
#define CORE_TRIANGULATION_EIGEN_H_

#include "core/triangulation.h"

namespace core {

/// Class for triangulation.
/// For more information about the algorithm, see
/// www.morethantechnical.com/2012/01/04/simple-triangulation-with-opencv-from-harley-zisserman-w-code  /NOLINT
class TriangulationEigen : public Triangulation {
 public:
  /// Constructor.
  TriangulationEigen();
  /// Destructor.
  ~TriangulationEigen() {}

  /// Sets Cameras member to cameras and computes inverted matrices.
  void SetCameraMatrices(const Cameras& cameras) override;

  /// Iterative eigen method for triangulation.
  /// @param[in] x1 Point2d on the first image.
  /// @param[in] x2 Point2d on the second image.
  /// @returns Triangulated Point3d point.
  cv::Point3d Triangulate(const cv::Point2d &x1,
      const cv::Point2d &x2) const override;
  /// Iterative eigen method for triangulation.
  /// @param[in] x1 Point2d on the first image.
  /// @param[in] x2 Point2d on the second image.
  /// @returns Triangulated depth of the point.
  double TriangulateDepth(const cv::Point2d &x1,
      const cv::Point2d &x2) const override;

 protected:
  /// Computes triangulation.
  /// @param[in] x1 Point3d projective coordinate of point on first image.
  /// @param[in] x2 Point3d projective coordinate of point on second image.
  /// @returns Mat_<double> 1x4 matrix of projective
  /// coordinate of the triangulated 3d point.
  cv::Mat_<double> LinearTriangulation(const cv::Point3d &x1,
                                       const cv::Point3d x2) const;
  /// Computes triangulation iterative.
  /// Calls LinearTriangulation function repeatedly,
  /// and selects the best result.
  /// @param[in] x1 Point3d projective coordinate of point x1 on first image.
  /// @param[in] x2 Point3d projective coordinate of point x2 on second image.
  cv::Mat_<double> IterativeTriangulation(const cv::Point3d& x1,
                                          const cv::Point3d& x2) const;

 private:
  /// Inverted camera one matrix.
  cv::Matx33d k_1_inv_;
  /// Inverted camera two matrix.
  cv::Matx33d k_2_inv_;

  /// Number of iterations.
  int iterations_;
  /// Value of the permissible error.
  double epsilon_;
};

}  // namespace core

#endif  // CORE_TRIANGULATION_EIGEN_H_
