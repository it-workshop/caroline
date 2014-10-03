// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_METRIC_IMPLS_H_
#define CORE_METRIC_IMPLS_H_

#include <string>
#include <vector>

#include "core/metric.h"

namespace core {

namespace stat {

const std::string kMean = "mean";
const std::string kSimpleDist = "simple_dist";
const std::string kRSquare = "RSquare";

/// Computes the mean value in matrix.
class Mean : public Metric {
 public:
  Mean()
  : Metric(kMean) {
  }

  long double compute(const std::vector<cv::Mat> &src) override;
};

/// Computes the sum of the roots of the difference of the squares.
class SimpleDist : public Metric {
 public:
  SimpleDist()
  : Metric(kSimpleDist) {
  }

  long double compute(const std::vector<cv::Mat> &src) override;
};

/// Computes the coefficient of determination.
class RSquare : public Metric {
 public:
  RSquare()
  : Metric(kRSquare) {
  }

  long double compute(const std::vector<cv::Mat> &src) override;
};

}  // namespace stat

}  // namespace core

#endif  // CORE_METRIC_IMPLS_H_
