// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "core/triangulation.h"

namespace core {

const int kIter = 10;
const double kEps = 0.05;

Triangulation::Triangulation() :
  iterations_(kIter),
  epsilon_(kEps) {
}

void Triangulation::SetCameraMatrices(const cv::Matx33d &K1,
                                      const cv::Matx33d &K2,
                                      const cv::Matx34d &P1,
                                      const cv::Matx34d &P2) {
  k_1_inv_ = K1.inv();
  k_2_inv_ = K2.inv();

  p_1_ = P1;
  p_2_ = P2;
}

void Triangulation::SetCameraMatrices(const Cameras &cameras) {
  SetCameraMatrices(cameras.K1(), cameras.K2(), cameras.P1(), cameras.P2());
}

cv::Point3d Triangulation::Triangulate(const cv::Point2d &x1,
                                       const cv::Point2d& x2) const {
  cv::Point3d point_one(x1.x, x1.y, 1.0);
  point_one = k_1_inv_ * point_one;

  cv::Point3d point_two(x2.x, x2.y, 1.0);
  point_two = k_2_inv_ * point_two;

  cv::Mat_<double> X = IterativeTriangulation(point_one, point_two);

  return cv::Point3d(X(0), X(1), X(2));
}

double Triangulation::TriangulateDepth(const cv::Point2d &x1,
                                       const cv::Point2d &x2) const {
  cv::Point3d point_one(x1.x, x1.y, 1.0);
  point_one = k_1_inv_ * point_one;

  cv::Point3d point_two(x2.x, x2.y, 1.0);
  point_two = k_2_inv_ * point_two;

  cv::Mat_<double> X = IterativeTriangulation(point_one, point_two);

  return X(2);
}

// From "Triangulation", Hartley, R.I. and Sturm, P.
// Computer vision and image understanding, 1997
cv::Mat_<double> Triangulation::LinearTriangulation(
    const cv::Point3d& x1,
    const cv::Point3d x2) const {
  cv::Matx43d A(x1.x * p_1_(2, 0) - p_1_(0, 0),
                x1.x * p_1_(2, 1) - p_1_(0, 1),
                x1.x * p_1_(2, 2) - p_1_(0, 2),
                x1.y * p_1_(2, 0) - p_1_(1, 0),
                x1.y * p_1_(2, 1) - p_1_(1, 1),
                x1.y * p_1_(2, 2) - p_1_(1, 2),
                x2.x * p_2_(2, 0) - p_2_(0, 0),
                x2.x * p_2_(2, 1) - p_2_(0, 1),
                x2.x * p_2_(2, 2) - p_2_(0, 2),
                x2.y * p_2_(2, 0) - p_2_(1, 0),
                x2.y * p_2_(2, 1) - p_2_(1, 1),
                x2.y * p_2_(2, 2) - p_2_(1, 2));

  cv::Mat_<double> B = (cv::Mat_<double>(4, 1) <<
                          -(x1.x * p_1_(2, 3) - p_1_(0, 3)),
                          -(x1.y * p_1_(2, 3) - p_1_(1, 3)),
                          -(x2.x * p_2_(2, 3) - p_2_(0, 3)),
                          -(x2.y * p_2_(2, 3) - p_2_(1, 3)));

  cv::Mat_<double> X;
  cv::solve(A, B, X, cv::DECOMP_SVD);

  return X;
}

// From "Triangulation", Hartley, R.I. and Sturm, P.
// Computer vision and image understanding, 1997
cv::Mat_<double> Triangulation::IterativeTriangulation(
            const cv::Point3d& x1, const cv::Point3d& x2) const {
  float wi1 = 1;
  float wi2 = 1;

  cv::Mat_<double> X(4, 1);

  for (int i = 0; i < iterations_; i++) {
    cv::Mat_<double> X_ = LinearTriangulation(x1, x2);
    X(0) = X_(0); X(1) = X_(1); X(2) = X_(2); X_(3) = 1.0;

    // calculate weights
    float p_2_x1 = static_cast<float>(
        cv::Mat_<double>(cv::Mat_<double>(p_1_).row(2)*X)(0));
    float p_2_x2 = static_cast<float>(
        cv::Mat_<double>(cv::Mat_<double>(p_2_).row(2)*X)(0));

    // breaking point
    if (fabsf(wi1 - p_2_x1) <= epsilon_ && fabsf(wi2 - p_2_x2) <= epsilon_)
      break;

    wi1 = p_2_x1;
    wi2 = p_2_x2;

    // reweight equations
    cv::Matx43d A(
          (x1.x * p_1_(2, 0) - p_1_(0, 0)) / wi1,
          (x1.x * p_1_(2, 1) - p_1_(0, 1)) / wi1,
          (x1.x * p_1_(2, 2) - p_1_(0, 2)) / wi1,
          (x1.y * p_1_(2, 0) - p_1_(1, 0)) / wi1,
          (x1.y * p_1_(2, 1) - p_1_(1, 1)) / wi1,
          (x1.y * p_1_(2, 2) - p_1_(1, 2)) / wi1,
          (x2.x * p_2_(2, 0) - p_2_(0, 0)) / wi2,
          (x2.x * p_2_(2, 1) - p_2_(0, 1)) / wi2,
          (x2.x * p_2_(2, 2) - p_2_(0, 2)) / wi2,
          (x2.y * p_2_(2, 0) - p_2_(1, 0)) / wi2,
          (x2.y * p_2_(2, 1) - p_2_(1, 1)) / wi2,
          (x2.y * p_2_(2, 2) - p_2_(1, 2)) / wi2);

    cv::Mat_<double> B =
        (cv::Mat_<double>(4, 1) <<
         -(x1.x * p_1_(2, 3) - p_1_(0, 3)) / wi1,
         -(x1.y * p_1_(2, 3) - p_1_(1, 3)) / wi1,
         -(x2.x * p_2_(2, 3) - p_2_(0, 3)) / wi2,
         -(x2.y * p_2_(2, 3) - p_2_(1, 3)) / wi2);

    cv::solve(A, B, X_, cv::DECOMP_SVD);
    X(0) = X_(0); X(1) = X_(1); X(2) = X_(2); X_(3) = 1.0;
    }

  return X;
}

}  // namespace core
