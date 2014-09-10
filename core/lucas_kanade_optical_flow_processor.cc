// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#include "core/lucas_kanade_optical_flow_processor.h"

#include <utility>
#include <vector>

#include "json/value.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/video.hpp"

namespace core {

namespace {

const char kMaxCornersNode[] = "max_corners";
const char kCornersQualityNode[] = "corners_quality";
const char kCornersDistanceNode[] = "corners_distance";
const char kSubPixSearchWindowHalfWidthNode[] =
    "sub_pix_search_window_half_width";
const char kSubPixSearchWindowHalfHeightNode[] =
    "sub_pix_search_window_half_height";
const char kSubPixZeroZoneHalfWidthNode[] = "sub_pix_zero_zone_half_width";
const char kSubPixZeroZoneHalfHeightNode[] = "sub_pix_zero_zone_half_height";
const char kSubPixIterationsNode[] = "sub_pix_iterations";
const char kSubPixEpsilonNode[] = "sub_pix_epsilon";
const char kWindowWidthNode[] = "window_width";
const char kWindowHeightNode[] = "window_height";
const char kMaxLevelNode[] = "max_level";
const char kOpticalFlowIterationsNode[] = "optical_flow_iterations";
const char kOpticalFlowEpsilonNode[] = "optical_flow_epsilon";

}  // namespace

// static
std::unique_ptr<OpticalFlowProcessor>
LucasKanadeOpticalFlowProcessor::Create(const Json::Value& settings) {
  std::unique_ptr<LucasKanadeOpticalFlowProcessor> processor(
      new LucasKanadeOpticalFlowProcessor());

  if (settings.isMember(kMaxCornersNode)) {
    const Json::Value& max_corners_count = settings[kMaxCornersNode];
    if (max_corners_count.isUInt64())
      processor->set_max_corners_count(max_corners_count.asUInt64());
  }

  if (settings.isMember(kCornersQualityNode)) {
    const Json::Value& corners_quality = settings[kCornersQualityNode];
    if (corners_quality.isDouble())
      processor->set_corners_quality(corners_quality.asDouble());
  }

  if (settings.isMember(kCornersDistanceNode)) {
    const Json::Value& corners_distance = settings[kCornersDistanceNode];
    if (corners_distance.isDouble())
      processor->set_corners_distance(corners_distance.asDouble());
  }

  if (settings.isMember(kSubPixSearchWindowHalfWidthNode)) {
    const Json::Value& sub_pix_search_window_half_width =
        settings[kSubPixSearchWindowHalfWidthNode];
    if (sub_pix_search_window_half_width.isUInt64())
      processor->set_sub_pix_search_window_half_width(
          sub_pix_search_window_half_width.asUInt64());
  }

  if (settings.isMember(kSubPixSearchWindowHalfHeightNode)) {
    const Json::Value& sub_pix_search_window_half_height =
        settings[kSubPixSearchWindowHalfHeightNode];
    if (sub_pix_search_window_half_height.isUInt64())
      processor->set_sub_pix_search_window_half_height(
          sub_pix_search_window_half_height.asUInt64());
  }

  if (settings.isMember(kSubPixZeroZoneHalfWidthNode)) {
    const Json::Value& sub_pix_zero_zone_half_width =
        settings[kSubPixZeroZoneHalfWidthNode];
    if (sub_pix_zero_zone_half_width.isUInt64())
      processor->set_sub_pix_zero_zone_half_width(
          sub_pix_zero_zone_half_width.asUInt64());
  }

  if (settings.isMember(kSubPixZeroZoneHalfHeightNode)) {
    const Json::Value& sub_pix_zero_zone_half_height =
        settings[kSubPixZeroZoneHalfHeightNode];
    if (sub_pix_zero_zone_half_height.isUInt64())
      processor->set_sub_pix_zero_zone_half_height(
          sub_pix_zero_zone_half_height.asUInt64());
  }

  if (settings.isMember(kSubPixIterationsNode)) {
    const Json::Value& sub_pix_iterations = settings[kSubPixIterationsNode];
    if (sub_pix_iterations.isDouble())
      processor->set_sub_pix_iterations(sub_pix_iterations.asUInt64());
  }

  if (settings.isMember(kSubPixEpsilonNode)) {
    const Json::Value& sub_pix_epsilon = settings[kSubPixEpsilonNode];
    if (sub_pix_epsilon.isDouble())
      processor->set_sub_pix_epsilon(sub_pix_epsilon.asDouble());
  }

  if (settings.isMember(kWindowWidthNode)) {
    const Json::Value& window_width = settings[kWindowWidthNode];
    if (window_width.isUInt64())
      processor->set_window_width(window_width.asUInt64());
  }

  if (settings.isMember(kWindowHeightNode)) {
    const Json::Value& window_height = settings[kWindowHeightNode];
    if (window_height.isUInt64())
      processor->set_window_height(window_height.asUInt64());
  }

  if (settings.isMember(kMaxLevelNode)) {
    const Json::Value& max_level = settings[kMaxLevelNode];
    if (max_level.isUInt64())
      processor->set_max_level(max_level.asUInt64());
  }

  if (settings.isMember(kOpticalFlowIterationsNode)) {
    const Json::Value& optical_flow_iterations =
        settings[kOpticalFlowIterationsNode];
    if (optical_flow_iterations.isUInt64())
      processor->set_optical_flow_iterations(
          optical_flow_iterations.asUInt64());
  }

  if (settings.isMember(kOpticalFlowEpsilonNode)) {
    const Json::Value& optical_flow_epsilon =
        settings[kOpticalFlowEpsilonNode];
    if (optical_flow_epsilon.isDouble())
      processor->set_optical_flow_epsilon(optical_flow_epsilon.asDouble());
  }

  return std::unique_ptr<OpticalFlowProcessor>(processor.release());
}

LucasKanadeOpticalFlowProcessor::LucasKanadeOpticalFlowProcessor()
  : max_corners_count_(500),
    corners_quality_(0.01),
    corners_distance_(5.0),
    sub_pix_search_window_half_width_(5),
    sub_pix_search_window_half_height_(5),
    sub_pix_zero_zone_half_width_(-1),
    sub_pix_zero_zone_half_height_(-1),
    sub_pix_iterations_(20),
    sub_pix_epsilon_(0.03),
    window_width_(21),
    window_height_(21),
    max_level_(3),
    optical_flow_iterations_(30),
    optical_flow_epsilon_(0.01) {
}

LucasKanadeOpticalFlowProcessor::~LucasKanadeOpticalFlowProcessor() {}

std::vector<std::pair<cv::Point2d, cv::Point2d>>
LucasKanadeOpticalFlowProcessor::Process(
    const cv::Mat& first, const cv::Mat& second) const {
  std::vector<cv::Point2d> corners;
  cv::goodFeaturesToTrack(
      first, corners, static_cast<int>(max_corners_count_), corners_quality_,
      corners_distance_);
  cv::cornerSubPix(
      first, corners,
      cv::Size(
          static_cast<int>(sub_pix_search_window_half_width_),
          static_cast<int>(sub_pix_search_window_half_height_)),
      cv::Size(
          static_cast<int>(sub_pix_zero_zone_half_width_),
          static_cast<int>(sub_pix_zero_zone_half_height_)),
      cv::TermCriteria(
          cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS,
          static_cast<int>(sub_pix_iterations_), sub_pix_epsilon_));
  std::vector<cv::Point2d> matches;
  std::vector<unsigned char> status;
  cv::Mat errors;
  cv::calcOpticalFlowPyrLK(
      first, second, corners, matches, status, errors,
      cv::Size(static_cast<int>(window_width_),
          static_cast<int>(window_height_)), static_cast<int>(max_level_),
      cv::TermCriteria(
      cv::TermCriteria::MAX_ITER | (cv::TermCriteria::EPS),
      static_cast<int>(optical_flow_iterations_), optical_flow_epsilon_));
  std::vector<std::pair<cv::Point2d, cv::Point2d>> result;
  for (size_t i = 0; i < corners.size(); ++i)
    if (status.at(i))
      result.push_back(std::make_pair(corners[i], matches[i]));
  return result;
}

}  // namespace core
