// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#include "core/lucas_kanade_optical_flow_processor.h"

#include <utility>
#include <vector>

#include "base/values.h"
#include "opencv2/core/mat.hpp"
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
const char kMaxLevel[] = "max_level";
const char kOpticalFlowIterationsNode[] = "optical_flow_iterations";
const char kOpticalFlowEpsilonNode[] = "optical_flow_epsilon";

}  // namespace

// static
std::unique_ptr<OpticalFlowProcessor>
LucasKanadeOpticalFlowProcessor::Create(const base::DictionaryValue* settings) {
  std::unique_ptr<LucasKanadeOpticalFlowProcessor> processor(
      new LucasKanadeOpticalFlowProcessor());

  auto max_corners_count = base::ToInteger(settings->GetValue(kMaxCornersNode));
  if (max_corners_count)
    processor->set_max_corners_count(max_corners_count->value());

  auto corners_quality = base::ToFloat(settings->GetValue(kCornersQualityNode));
  if (corners_quality)
    processor->set_corners_quality(corners_quality->value());

  auto corners_distance =
      base::ToFloat(settings->GetValue(kCornersDistanceNode));
  if (corners_distance)
    processor->set_corners_distance(corners_distance->value());

  auto sub_pix_search_window_half_width =
      base::ToInteger(settings->GetValue(kSubPixSearchWindowHalfWidthNode));
  if (sub_pix_search_window_half_width)
    processor->set_sub_pix_search_window_half_width(
        sub_pix_search_window_half_width->value());

  auto sub_pix_search_window_half_height =
      base::ToInteger(settings->GetValue(kSubPixSearchWindowHalfHeightNode));
  if (sub_pix_search_window_half_height)
    processor->set_sub_pix_search_window_half_height(
        sub_pix_search_window_half_height->value());

  auto sub_pix_zero_zone_half_width =
      base::ToInteger(settings->GetValue(kSubPixZeroZoneHalfWidthNode));
  if (sub_pix_zero_zone_half_width)
    processor->set_sub_pix_zero_zone_half_width(
        sub_pix_zero_zone_half_width->value());

  auto sub_pix_zero_zone_half_height =
      base::ToInteger(settings->GetValue(kSubPixZeroZoneHalfHeightNode));
  if (sub_pix_zero_zone_half_height)
    processor->set_sub_pix_zero_zone_half_height(
        sub_pix_zero_zone_half_height->value());

  auto sub_pix_iterations = base::ToInteger(
      settings->GetValue(kSubPixIterationsNode));
  if (sub_pix_iterations)
    processor->set_sub_pix_iterations(sub_pix_iterations->value());

  auto sub_pix_epsilon =
      base::ToFloat(settings->GetValue(kSubPixEpsilonNode));
  if (sub_pix_epsilon)
    processor->set_sub_pix_epsilon(sub_pix_epsilon->value());

  auto window_width = base::ToInteger(settings->GetValue(kWindowWidthNode));
  if (window_width)
    processor->set_window_width(window_width->value());

  auto window_height = base::ToInteger(settings->GetValue(kWindowHeightNode));
  if (window_height)
    processor->set_window_height(window_height->value());

  auto max_level = base::ToInteger(settings->GetValue(kMaxLevel));
  if (max_level)
    processor->set_max_level(max_level->value());

  auto optical_flow_iterations =
      base::ToInteger(settings->GetValue(kOpticalFlowIterationsNode));
  if (optical_flow_iterations)
    processor->set_optical_flow_iterations(optical_flow_iterations->value());

  auto optical_flow_epsilon =
      base::ToFloat(settings->GetValue(kOpticalFlowEpsilonNode));
  if (optical_flow_epsilon)
    processor->set_optical_flow_epsilon(optical_flow_epsilon->value());

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
  std::vector<cv::Point2f> corners;
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
  std::vector<cv::Point2f> matches;
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
