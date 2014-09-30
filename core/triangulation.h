// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_TRIANGULATION_H_
#define CORE_TRIANGULATION_H_

#include "core/cameras.h"
#include "opencv2/core/core.hpp"

namespace core {

/// Abstract class for triangulation.
class Triangulation {
 public:
  /// Constructor.
  Triangulation() {}

  /// Destructor.
  virtual ~Triangulation() {}

  /// Sets the cameras_ = cameras.
  virtual void SetCameraMatrices(const Cameras& cameras) { cameras_ = cameras; }
  /// Virtual method for triangulation.
  /// @param[in] x1 Point2d on the first image.
  /// @param[in] x2 Point2d on the second image.
  /// @returns Triangulated Point3d point.
  virtual cv::Point3d Triangulate(const cv::Point2d &x1,
      const cv::Point2d &x2) const = 0;
  /// Virtual method for triangulation.
  /// @param[in] x1 Point2d on the first image.
  /// @param[in] x2 Point2d on the second image.
  /// @returns Triangulated depth of the point.
  virtual double TriangulateDepth(const cv::Point2d &x1,
      const cv::Point2d &x2) const = 0;

 protected:
  /// Returns camera one matrix.
  cv::Matx33d K1() const { return cameras_.K1(); }
  /// Returns camera two matrix.
  cv::Matx33d K2() const { return cameras_.K2(); }
  /// Returns projective matrix of camera one.
  cv::Matx34d P1() const { return cameras_.P1(); }
  /// Returns projective matrix of camera two.
  cv::Matx34d P2() const { return cameras_.P2(); }

 private:
  /// Parameters of the cameras.
  Cameras cameras_;
};

}  // namespace core

#endif  // CORE_TRIANGULATION_H_
