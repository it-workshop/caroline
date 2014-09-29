// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "core/triangulation_eigen.h"

namespace core {

const int kIter = 10;
const double kEps = 0.005;

TriangulationEigen::TriangulationEigen() :
  iterations_(kIter),
  epsilon_(kEps) {
}

void TriangulationEigen::SetCameraMatrices(const Cameras& cameras) {
  Triangulation::SetCameraMatrices(cameras);
  k_1_inv_ = cameras.K1().inv();
  k_2_inv_ = cameras.K2().inv();
}

cv::Point3d TriangulationEigen::Triangulate(const cv::Point2d &x1,
                                       const cv::Point2d &x2) const {
  cv::Point3d point_one(x1.x, x1.y, 1.0);
  point_one = k_1_inv_ * point_one;

  cv::Point3d point_two(x2.x, x2.y, 1.0);
  point_two = k_2_inv_ * point_two;

  cv::Mat_<double> X = IterativeTriangulation(point_one, point_two);

  return cv::Point3d(X(0), X(1), X(2));
}

double TriangulationEigen::TriangulateDepth(const cv::Point2d &x1,
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
cv::Mat_<double> TriangulationEigen::LinearTriangulation(
    const cv::Point3d& x1,
    const cv::Point3d x2) const {
  cv::Matx43d A(x1.x * P1()(2, 0) - P1()(0, 0),
                x1.x * P1()(2, 1) - P1()(0, 1),
                x1.x * P1()(2, 2) - P1()(0, 2),
                x1.y * P1()(2, 0) - P1()(1, 0),
                x1.y * P1()(2, 1) - P1()(1, 1),
                x1.y * P1()(2, 2) - P1()(1, 2),
                x2.x * P2()(2, 0) - P2()(0, 0),
                x2.x * P2()(2, 1) - P2()(0, 1),
                x2.x * P2()(2, 2) - P2()(0, 2),
                x2.y * P2()(2, 0) - P2()(1, 0),
                x2.y * P2()(2, 1) - P2()(1, 1),
                x2.y * P2()(2, 2) - P2()(1, 2));

  cv::Mat_<double> B = (cv::Mat_<double>(4, 1) <<
                          -(x1.x * P1()(2, 3) - P1()(0, 3)),
                          -(x1.y * P1()(2, 3) - P1()(1, 3)),
                          -(x2.x * P2()(2, 3) - P2()(0, 3)),
                          -(x2.y * P2()(2, 3) - P2()(1, 3)));

  cv::Mat_<double> X;
  cv::solve(A, B, X, cv::DECOMP_SVD);

  return X;
}

// From "Triangulation", Hartley, R.I. and Sturm, P.
// Computer vision and image understanding, 1997
cv::Mat_<double> TriangulationEigen::IterativeTriangulation(
            const cv::Point3d& x1, const cv::Point3d& x2) const {
  float wi1 = 1;
  float wi2 = 1;

  cv::Mat_<double> X(4, 1);

  for (int i = 0; i < iterations_; i++) {
    cv::Mat_<double> X_ = LinearTriangulation(x1, x2);
    X(0) = X_(0); X(1) = X_(1); X(2) = X_(2); X_(3) = 1.0;

    // calculate weights
    float p_2_x1 = static_cast<float>(
        cv::Mat_<double>(cv::Mat_<double>(P1()).row(2)*X)(0));
    float p_2_x2 = static_cast<float>(
        cv::Mat_<double>(cv::Mat_<double>(P2()).row(2)*X)(0));

    // breaking point
    if (fabsf(wi1 - p_2_x1) <= epsilon_ && fabsf(wi2 - p_2_x2) <= epsilon_)
      break;

    wi1 = p_2_x1;
    wi2 = p_2_x2;

    // reweight equations
    cv::Matx43d A(
          (x1.x * P1()(2, 0) - P1()(0, 0)) / wi1,
          (x1.x * P1()(2, 1) - P1()(0, 1)) / wi1,
          (x1.x * P1()(2, 2) - P1()(0, 2)) / wi1,
          (x1.y * P1()(2, 0) - P1()(1, 0)) / wi1,
          (x1.y * P1()(2, 1) - P1()(1, 1)) / wi1,
          (x1.y * P1()(2, 2) - P1()(1, 2)) / wi1,
          (x2.x * P2()(2, 0) - P2()(0, 0)) / wi2,
          (x2.x * P2()(2, 1) - P2()(0, 1)) / wi2,
          (x2.x * P2()(2, 2) - P2()(0, 2)) / wi2,
          (x2.y * P2()(2, 0) - P2()(1, 0)) / wi2,
          (x2.y * P2()(2, 1) - P2()(1, 1)) / wi2,
          (x2.y * P2()(2, 2) - P2()(1, 2)) / wi2);

    cv::Mat_<double> B =
        (cv::Mat_<double>(4, 1) <<
         -(x1.x * P1()(2, 3) - P1()(0, 3)) / wi1,
         -(x1.y * P1()(2, 3) - P1()(1, 3)) / wi1,
         -(x2.x * P2()(2, 3) - P2()(0, 3)) / wi2,
         -(x2.y * P2()(2, 3) - P2()(1, 3)) / wi2);

    cv::solve(A, B, X_, cv::DECOMP_SVD);
    X(0) = X_(0); X(1) = X_(1); X(2) = X_(2); X_(3) = 1.0;
    }

  return X;
}

}  // namespace core
