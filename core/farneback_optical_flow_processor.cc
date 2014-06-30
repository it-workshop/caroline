// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Sergey Volodin <etoestja@yandex.ru>

#include "core/farneback_optical_flow_processor.h"

#include <utility>
#include <vector>

#include "core/config.h"

#include "base/values.h"

#include "opencv2/core/mat.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/video.hpp"

namespace core {

namespace {
const char kPyrScaleNode[] = "pyr_scale";
const char kLevelsNode[] = "levels";
const char kWinsizeNode[] = "winsize";
const char kIterationsNode[] = "iterations";
const char kPolyNNode[] = "poly_n";
const char kPolySigmaNode[] = "poly_sigma";
const char kFlagsNode[] = "flags";
}

// static
std::unique_ptr<OpticalFlowProcessor>
FarnebackOpticalFlowProcessor::Create(const base::DictionaryValue *settings) {
  std::unique_ptr<FarnebackOpticalFlowProcessor> processor(
        new FarnebackOpticalFlowProcessor());

  auto pyr_scale = base::ToFloat(settings->GetValue(kPyrScaleNode));
  if (pyr_scale)
    processor->set_pyr_scale(pyr_scale->value());

  auto levels = base::ToInteger(settings->GetValue(kLevelsNode));
  if (levels)
    processor->set_levels(levels->value());

  auto winsize = base::ToInteger(settings->GetValue(kWinsizeNode));
  if (winsize)
    processor->set_winsize(winsize->value());

  auto iterations = base::ToInteger(settings->GetValue(kIterationsNode));
  if (iterations)
    processor->set_iterations(iterations->value());

  auto poly_n = base::ToInteger(settings->GetValue(kPolyNNode));
  if (poly_n)
    processor->set_poly_n(poly_n->value());

  auto poly_sigma = base::ToFloat(settings->GetValue(kPolySigmaNode));
  if (poly_sigma)
    processor->set_poly_sigma(poly_sigma->value());

  auto flags = base::ToInteger(settings->GetValue(kFlagsNode));
  if (flags)
    processor->set_flags(flags->value());

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

  // cv::cvtColor(first, first_gray, COLOR_BGR2GRAY);
  // cv::cvtColor(second, second_gray, COLOR_BGR2GRAY);

  cv::calcOpticalFlowFarneback(first_gray, second_gray, flow,
                               pyr_scale_, levels_, winsize_, iterations_,
                               poly_n_, poly_sigma_, flags_);

  std::vector< std::pair<cv::Point2d, cv::Point2d> > v;

  for (int y = 0; y < first_gray.rows; y++)
    for (int x = 0; x < first_gray.cols; x++) {
      const cv::Point2f& f = flow.at<cv::Point2f>(y, x);

      cv::Point2d p1 = cv::Point2d(x, y);
      cv::Point2d p2 = cv::Point2d(cvRound(x + f.x), cvRound(y + f.y));

      /* if (!(p2.y >= 0 && p2.y < second_gray.rows &&
          p2.x >= 0 && p2.x < second_gray.cols))
        std::cerr << "overflow" << std::endl; */

      v.push_back(std::make_pair(p1, p2));
    }

  return(v);
}

}  // namespace core
