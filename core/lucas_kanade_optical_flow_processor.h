// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#ifndef CORE_LUCAS_KANADE_OPTICAL_FLOW_PROCESSOR_H_
#define CORE_LUCAS_KANADE_OPTICAL_FLOW_PROCESSOR_H_

#include <utility>
#include <vector>

#include "core/optical_flow_processor.h"

namespace base {

class DictionaryValue;

}  // namespace base

namespace core {

class LucasKanadeOpticalFlowProcessor : public OpticalFlowProcessor {
 public:
  static std::unique_ptr<OpticalFlowProcessor>
  Create(const base::DictionaryValue* settings);

  LucasKanadeOpticalFlowProcessor();
  virtual ~LucasKanadeOpticalFlowProcessor();

  virtual std::vector<std::pair<cv::Point2d, cv::Point2d>> Process(
      const cv::Mat& first,
      const cv::Mat& second) const override;

  int64_t max_corners_count() const {
    return max_corners_count_;
  }
  void set_max_corners_count(int64_t v) {
    max_corners_count_ = v;
  }
  double corners_quality() const {
    return corners_quality_;
  }
  void set_corners_quality(double v) {
    corners_quality_ = v;
  }
  double corners_distance() const {
    return corners_distance_;
  }
  void set_corners_distance(double v) {
    corners_distance_ = v;
  }
  int64_t sub_pix_search_window_half_width() const {
    return sub_pix_search_window_half_width_;
  }
  void set_sub_pix_search_window_half_width(int64_t v) {
    sub_pix_search_window_half_width_ = v;
  }
  int64_t sub_pix_search_window_half_height() const {
    return sub_pix_search_window_half_height_;
  }
  void set_sub_pix_search_window_half_height(int64_t v) {
    sub_pix_search_window_half_height_ = v;
  }
  int64_t sub_pix_zero_zone_half_width() const {
    return sub_pix_zero_zone_half_width_;
  }
  void set_sub_pix_zero_zone_half_width(int64_t v) {
    sub_pix_zero_zone_half_width_ = v;
  }
  int64_t sub_pix_zero_zone_half_height() const {
    return sub_pix_zero_zone_half_height_;
  }
  void set_sub_pix_zero_zone_half_height(int64_t v) {
    sub_pix_zero_zone_half_height_ = v;
  }
  int64_t sub_pix_iterations() const {
    return sub_pix_iterations_;
  }
  void set_sub_pix_iterations(int64_t v) {
    sub_pix_iterations_ = v;
  }
  double sub_pix_epsilon() const {
    return sub_pix_epsilon_;
  }
  void set_sub_pix_epsilon(double v) {
    sub_pix_epsilon_ = v;
  }
  int64_t window_width() const {
    return window_width_;
  }
  void set_window_width(int64_t v) {
    window_width_ = v;
  }
  int64_t window_height() const {
    return window_height_;
  }
  void set_window_height(int64_t v) {
    window_height_ = v;
  }
  int64_t max_level() const {
    return max_level_;
  }
  void set_max_level(int64_t v) {
    max_level_ = v;
  }
  int64_t optical_flow_iterations() const {
    return optical_flow_iterations_;
  }
  void set_optical_flow_iterations(int64_t v) {
    optical_flow_iterations_ = v;
  }
  double optical_flow_epsilon() const {
    return optical_flow_epsilon_;
  }
  void set_optical_flow_epsilon(double v) {
    optical_flow_epsilon_ = v;
  }

 private:
  int64_t max_corners_count_;
  double corners_quality_;
  double corners_distance_;
  int64_t sub_pix_search_window_half_width_;
  int64_t sub_pix_search_window_half_height_;
  int64_t sub_pix_zero_zone_half_width_;
  int64_t sub_pix_zero_zone_half_height_;
  int64_t sub_pix_iterations_;
  double sub_pix_epsilon_;
  int64_t window_width_;
  int64_t window_height_;
  int64_t max_level_;
  int64_t optical_flow_iterations_;
  double optical_flow_epsilon_;
};

}  // namespace core

#endif  // CORE_LUCAS_KANADE_OPTICAL_FLOW_PROCESSOR_H_
