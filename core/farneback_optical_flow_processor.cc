// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Sergey Volodin <etoestja@yandex.ru>

#include "core/farneback_optical_flow_processor.h"

#include <utility>
#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/video.hpp"
#include "json/value.h"

namespace core {

namespace {

const char kPyrScaleNode[] = "pyr_scale";
const char kLevelsNode[] = "levels";
const char kWinsizeNode[] = "winsize";
const char kIterationsNode[] = "iterations";
const char kPolyNNode[] = "poly_n";
const char kPolySigmaNode[] = "poly_sigma";
const char kFlagsNode[] = "flags";

}  // namespace

// static
std::unique_ptr<OpticalFlowProcessor>
FarnebackOpticalFlowProcessor::Create(const Json::Value& settings) {
  std::unique_ptr<FarnebackOpticalFlowProcessor> processor(
        new FarnebackOpticalFlowProcessor());

  if (settings.isMember(kPyrScaleNode)) {
    const Json::Value& pyr_scale = settings[kPyrScaleNode];
    if (pyr_scale.isDouble())
      processor->set_pyr_scale(pyr_scale.asDouble());
  }

  if (settings.isMember(kLevelsNode)) {
    const Json::Value& levels = settings[kLevelsNode];
    if (levels.isUInt64())
      processor->set_levels(levels.asUInt64());
  }

  if (settings.isMember(kWinsizeNode)) {
    const Json::Value& winsize = settings[kWinsizeNode];
    if (winsize.isUInt64())
      processor->set_winsize(winsize.asUInt64());
  }

  if (settings.isMember(kIterationsNode)) {
    const Json::Value& iterations = settings[kIterationsNode];
    if (iterations.isUInt64())
      processor->set_iterations(iterations.asUInt64());
  }

  if (settings.isMember(kPolyNNode)) {
    const Json::Value& poly_n = settings[kPolyNNode];
    if (poly_n.isUInt64())
      processor->set_poly_n(poly_n.asUInt64());
  }

  if (settings.isMember(kPolySigmaNode)) {
    const Json::Value& poly_sigma = settings[kPolySigmaNode];
    if (poly_sigma.isUInt64())
      processor->set_poly_sigma(poly_sigma.asUInt64());
  }

  if (settings.isMember(kFlagsNode)) {
    const Json::Value& flags = settings[kFlagsNode];
    if (flags.isUInt64())
      processor->set_flags(flags.asUInt64());
  }

  return std::unique_ptr<OpticalFlowProcessor>(processor.release());
}

FarnebackOpticalFlowProcessor::FarnebackOpticalFlowProcessor()
  : pyr_scale_(0.5),
    levels_(3),
    winsize_(15),
    iterations_(3),
    poly_n_(5),
    poly_sigma_(1.2),
    flags_(0) {
}

std::vector< std::pair<cv::Point2d, cv::Point2d> >
FarnebackOpticalFlowProcessor::Process(
    const cv::Mat& first, const cv::Mat& second) const {
  cv::Mat first_gray = first, second_gray = second, flow;

  cv::calcOpticalFlowFarneback(first_gray, second_gray, flow,
                               pyr_scale_, static_cast<int>(levels_),
                               static_cast<int>(winsize_),
                               static_cast<int>(iterations_),
                               static_cast<int>(poly_n_), poly_sigma_,
                               static_cast<int>(flags_));

  std::vector< std::pair<cv::Point2d, cv::Point2d> > v;

  for (int y = 0; y < first_gray.rows; y++)
    for (int x = 0; x < first_gray.cols; x++) {
      const cv::Point2f& f = flow.at<cv::Point2f>(y, x);

      cv::Point2d p1 = cv::Point2d(x, y);
      cv::Point2d p2 = cv::Point2d(cvRound(x + f.x), cvRound(y + f.y));

      v.push_back(std::make_pair(p1, p2));
    }

  return(v);
}

}  // namespace core
