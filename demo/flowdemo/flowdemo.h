// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>
// Author: Sergey Volodin <etoestja@yandex.ru>

#ifndef DEMO_FLOWDEMO_FLOWDEMO_H_
#define DEMO_FLOWDEMO_FLOWDEMO_H_

#include <memory>
#include <vector>
#include <utility>

#include "opencv2/core/mat.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

namespace base {

class CommandLine;

}  // namespace base

namespace core {

class Config;
class ImageCaptureManager;
class OpticalFlowProcessor;

}  // namespace core

namespace demo {

class FlowDemo {
 public:
  FlowDemo(base::CommandLine* command_line, core::Config* config);
  virtual ~FlowDemo();

  bool Init();

  int Run();

 private:
  void DrawOptFlowMap();

  base::CommandLine* command_line_;
  core::Config* config_;
  std::unique_ptr<core::ImageCaptureManager> image_capture_manager_;
  std::unique_ptr<core::OpticalFlowProcessor> optical_frow_processor_;

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

#endif  // DEMO_FLOWDEMO_FLOWDEMO_H_
