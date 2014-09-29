// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_TRIANGULATION_H_
#define CORE_TRIANGULATION_H_

#include "core/cameras.h"
#include "opencv2/core/core.hpp"

namespace core {

class Triangulation {
 public:
  Triangulation() {}

  virtual ~Triangulation() {}

  virtual void SetCameraMatrices(const Cameras& cameras) { cameras_ = cameras; }

  virtual cv::Point3d Triangulate(const cv::Point2d &x1,
      const cv::Point2d &x2) const = 0;
  virtual double TriangulateDepth(const cv::Point2d &x1,
      const cv::Point2d &x2) const = 0;

 protected:
  cv::Matx33d K1() const { return cameras_.K1(); }
  cv::Matx33d K2() const { return cameras_.K2(); }
  cv::Matx34d P1() const { return cameras_.P1(); }
  cv::Matx34d P2() const { return cameras_.P2(); }

 private:
  Cameras cameras_;
};

}  // namespace core

#endif  // CORE_TRIANGULATION_H_
