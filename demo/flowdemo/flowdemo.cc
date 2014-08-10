// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>
// Author: Sergey Volodin <etoestja@yandex.ru>

#include "demo/flowdemo/flowdemo.h"

#include <utility>
#include <algorithm>

#include "core/config.h"
#include "core/image_capture_manager.h"
#include "core/optical_flow_processor.h"
#include "core/position.h"
#include "core/time_controller.h"
#include "opencv2/core/mat.hpp"

namespace demo {

namespace {

const char kFlowDemoNode[] = "demo_flowdemo";
const char kResizeFactorXNode[] = "resize_factor_x";
const char kResizeFactorYNode[] = "resize_factor_x";
const char kStepNode[] = "step";
const char kCapNumberNode[] = "cap_number";

}  // namespace

void FlowDemo::DrawOptFlowMap() {
  std::pair<cv::Point2d, cv::Point2d> p;

  for (size_t i = 0; i < flow_.size(); ) {
    p = flow_[i];
    cv::line(output_, p.first, p.second, color_);
    cv::circle(output_, p.first, 1, color_, -1);

    if ((i / output_.cols) != ((i + step_) / output_.cols)) {
      i = output_.cols * (i / output_.cols);
      i += step_ * output_.cols;
    } else {
      i += step_;
    }
  }
}


FlowDemo::FlowDemo(base::CommandLine* command_line, core::Config* config)
  : command_line_(command_line),
    config_(config),
    resize_factor_x_(0.3),
    resize_factor_y_(0.3),
    color_(cv::Scalar(0, 255, 0)),
    step_(7),
    cap_number_(0) {
  if (!config)
    return;

  const Json::Value* dictionary = config->dictionary();
  if (!dictionary || !dictionary->isMember(kFlowDemoNode))
    return;

  const Json::Value& settings = (*dictionary)[kFlowDemoNode];
  if (!settings.isObject())
    return;

  if (settings.isMember(kResizeFactorXNode)) {
    const Json::Value& resize_factor_x = settings[kResizeFactorXNode];
    if (resize_factor_x.isDouble())
      resize_factor_x_ = resize_factor_x.asDouble();
  }

  if (settings.isMember(kResizeFactorYNode)) {
    const Json::Value& resize_factor_y = settings[kResizeFactorYNode];
    if (resize_factor_y.isDouble())
      resize_factor_y_ = resize_factor_y.asDouble();
  }

  if (settings.isMember(kCapNumberNode)) {
    const Json::Value& cap_number = settings[kCapNumberNode];
    if (cap_number.isUInt64())
      cap_number_ = cap_number.asUInt64();
  }

  if (settings.isMember(kStepNode)) {
    const Json::Value& step = settings[kStepNode];
    if (step.isUInt64())
      step_ = step.asUInt64();
  }
}

FlowDemo::~FlowDemo() {}

bool FlowDemo::Init() {
  cap_ = cv::VideoCapture(cap_number_);

  image_capture_manager_ = core::ImageCaptureManager::Create(config_);
  optical_flow_processor_ = core::OpticalFlowProcessor::Create(config_);

  return optical_flow_processor_ && cap_.isOpened();
}

int FlowDemo::Run() {
  cv::namedWindow("flow", 1);

  for ( ; ; ) {
    cap_ >> frame_;

    cv::resize(frame_, frame_resized_, cv::Size(),
               resize_factor_x_, resize_factor_y_);
    cv::cvtColor(frame_resized_, gray_, cv::COLOR_BGR2GRAY);

    if (prevgray_.data) {
      flow_ = optical_flow_processor_->Process(prevgray_, gray_);
      cv::cvtColor(prevgray_, output_, cv::COLOR_GRAY2BGR);
      DrawOptFlowMap();
      cv::imshow("flow", output_);
    }

    cv::waitKey(1);

    std::swap(prevgray_, gray_);
  }

  return 0;
}

}  // namespace demo
