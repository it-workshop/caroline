// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Sergey Volodin <etoestja@yandex.ru>

#ifndef CORE_FARNEBACK_OPTICAL_FLOW_PROCESSOR_H_
#define CORE_FARNEBACK_OPTICAL_FLOW_PROCESSOR_H_

#include <utility>
#include <vector>

#include "core/optical_flow_processor.h"

namespace Json {

class Value;

}  // namespace Json

namespace core {

class FarnebackOpticalFlowProcessor : public OpticalFlowProcessor {
 public:
  static std::unique_ptr<OpticalFlowProcessor>
  Create(const Json::Value& settings);

  FarnebackOpticalFlowProcessor();
  virtual ~FarnebackOpticalFlowProcessor() {}

  std::vector< std::pair<cv::Point2d, cv::Point2d> > Process(
      const cv::Mat& first,
      const cv::Mat& second) const override;

  double pyr_scale() const {
    return(pyr_scale_);
  }
  void set_pyr_scale(double v) {
    pyr_scale_ = v;
  }

  int64_t levels() const {
    return(levels_);
  }
  void set_levels(int64_t v) {
    levels_ = v;
  }

  int64_t winsize() const {
    return(winsize_);
  }
  void set_winsize(int64_t v) {
    winsize_ = v;
  }

  int64_t iterations() const {
    return(iterations_);
  }
  void set_iterations(int64_t v) {
    iterations_ = v;
  }

  int64_t poly_n() const {
    return(poly_n_);
  }
  void set_poly_n(int64_t v) {
    poly_n_ = v;
  }

  double poly_sigma() const {
    return(poly_sigma_);
  }
  void set_poly_sigma(double v) {
    poly_sigma_ = v;
  }

  int64_t flags() const {
    return(flags_);
  }
  void set_flags(int64_t v) {
    flags_ = v;
  }

 private:
  double pyr_scale_;
  int64_t levels_;
  int64_t winsize_;
  int64_t iterations_;
  int64_t poly_n_;
  double poly_sigma_;
  int64_t flags_;
};

}  // namespace core

#endif  // CORE_FARNEBACK_OPTICAL_FLOW_PROCESSOR_H_
