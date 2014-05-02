// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include <math.h>

#include "core/depth_map_builder.h"

namespace core {

// set your own epsilon
const double epsilon = 0.05;

// suggests 10 at most
const int iterations = 10;

DepthMapBuilder::DepthMapBuilder() {
}


// From "Triangulation", Hartley, R.I. and Sturm, P., Computer vision and image understanding, 1997
cv::Mat_<double> DepthMapBuilder::LinearTriangulation(cv::Point3d x1,
                                              cv::Matx34d P1,
                                              cv::Point3d x2,
                                              cv::Matx34d P2) {
  cv::Matx43d A(x1.x*P1(2,0)-P1(0,0), x1.x*P1(2,1)-P1(0,1), x1.x*P1(2,2)-P1(0,2),
                x1.y*P1(2,0)-P1(1,0), x1.y*P1(2,1)-P1(1,1), x1.y*P1(2,2)-P1(1,2),
                x2.x*P2(2,0)-P2(0,0), x2.x*P2(2,1)-P2(0,1), x2.x*P2(2,2)-P2(0,2),
                x2.y*P2(2,0)-P2(1,0), x2.y*P2(2,1)-P2(1,1), x2.y*P2(2,2)-P2(1,2)
                );

  cv::Mat_<double> B = (cv::Mat_<double>(4,1) << 
                          -(x1.x*P1(2,3) -P1(0,3)),
                          -(x1.y*P1(2,3) -P1(1,3)),
                          -(x2.x*P2(2,3) -P2(0,3)),
                          -(x2.y*P2(2,3) -P2(1,3))
                        );

  cv::Mat_<double> X;
  cv::solve(A, B, X, cv::DECOMP_SVD);

  return X;
}

// From "Triangulation", Hartley, R.I. and Sturm, P., Computer vision and image understanding, 1997
cv::Mat_<double> DepthMapBuilder::IterativeTriangulation(cv::Point3d x1,
                                                         cv::Matx34d P1,
                                                         cv::Point3d x2,
                                                         cv::Matx34d P2) {
  double wi1 = 1;
  double wi2 = 1;

  cv::Mat_<double> X(4,1);

  for (int i = 0; i < iterations; i++) {
    cv::Mat_<double> X_ = LinearTriangulation(x1,P1,x2,P2);
    X(0) = X_(0); X(1) = X_(1); X(2) = X_(2); X_(3) = 1.0;

    // calculate weights
    double p2x1 = cv::Mat_<double>(cv::Mat_<double>(P1).row(2)*X)(0);
    double p2x2 = cv::Mat_<double>(cv::Mat_<double>(P2).row(2)*X)(0);

    // breaking point
    if (fabsf(wi1 - p2x1) <=  epsilon && fabsf(wi2 - p2x2) <= epsilon) break;

    wi1 = p2x1;
    wi2 = p2x2;

    // reweight equations
    cv::Matx43d A(
          (x1.x*P1(2,0)-P1(0,0))/wi1, (x1.x*P1(2,1)-P1(0,1))/wi1, (x1.x*P1(2,2)-P1(0,2))/wi1,
          (x1.y*P1(2,0)-P1(1,0))/wi1, (x1.y*P1(2,1)-P1(1,1))/wi1, (x1.y*P1(2,2)-P1(1,2))/wi1,
          (x2.x*P2(2,0)-P2(0,0))/wi2, (x2.x*P2(2,1)-P2(0,1))/wi2, (x2.x*P2(2,2)-P2(0,2))/wi2,
          (x2.y*P2(2,0)-P2(1,0))/wi2, (x2.y*P2(2,1)-P2(1,1))/wi2, (x2.y*P2(2,2)-P2(1,2))/wi2
          );

    cv::Mat_<double> B =
        (cv::Mat_<double>(4,1) << -(x1.x*P1(2,3) -P1(0,3))/wi1,
                                  -(x1.y*P1(2,3) -P1(1,3))/wi1,
                                  -(x2.x*P2(2,3) -P2(0,3))/wi2,
                                  -(x2.y*P2(2,3) -P2(1,3))/wi2
        );

    cv::solve(A,B,X_,cv::DECOMP_SVD);
    X(0) = X_(0); X(1) = X_(1); X(2) = X_(2); X_(3) = 1.0;
    }

  return X;
}

}  // namespace core
