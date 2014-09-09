// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_METRICS_H_
#define CORE_METRICS_H_

#include "core/metric.h"

namespace core {

namespace stat {

/// Computes the sum of the roots of the difference of the squares.
class SimpleDist : public Metric {
 public:
  SimpleDist() :
    Metric("simple_dist") {
  }

  double compute(const cv::Mat &m1, const cv::Mat &m2) override;
};

/// Computes the coefficient of determination.
class RSquare : public Metric {
 public:
  RSquare() :
    Metric("RSquare") {
  }

  double compute(const cv::Mat &m1, const cv::Mat &m2) override;
};

}  // namespace stat

}  // namespace core

#endif  // CORE_METRICS_H_
