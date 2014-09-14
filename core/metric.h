// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_METRIC_H_
#define CORE_METRIC_H_

#include <string>

namespace cv {
class Mat;
}

namespace core {

namespace stat {

/// Abstract class for different metrics for comparison matrices.
class Metric {
 public:
  /// Constructor.
  explicit Metric(const std::string& name)
  : name_(name) {
  }

  /// Virtual destructor.
  virtual ~Metric() {}

  /// Returns the name of the metric.
  std::string name() const { return name_; }

  /// Computes the value for comparison matrices.
  virtual long double compute(const cv::Mat& m1, const cv::Mat& m2) = 0;

 private:
  /// Name of the metric.
  const std::string name_;
};

}  // namespace stat

}  // namespace core

#endif  // CORE_METRIC_H_
