// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>
// Author: Sergey Volodin <etoestja@yandex.ru>

#ifndef CORE_DEMO_FLOWDEMO_H_
#define CORE_DEMO_FLOWDEMO_H_

#include <memory>
#include <vector>
#include <utility>

#include "core/caroline.h"
#include "opencv2/core/core.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

namespace demo {

class FlowDemo : public core::Caroline {
 public:
  explicit FlowDemo(base::CommandLine* command_line);
  virtual ~FlowDemo();

  virtual bool Init() override;

  virtual int Run() override;

  static const char kDemoName[];

 private:
  void DrawOptFlowMap();

  double resize_factor_x_;
  double resize_factor_y_;

  int cap_number_;

  cv::Scalar color_;
  int step_;

  cv::VideoCapture cap_;

  cv::Mat prevgray_, gray_, output_, frame_resized_, frame_;

  std::vector< std::pair<cv::Point2d, cv::Point2d> > flow_;
};

}  // namespace demo

#endif  // CORE_DEMO_FLOWDEMO_H_
