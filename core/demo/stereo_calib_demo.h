// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Kukleva Anna <Kuklevaanna@gmail.com>


#ifndef CORE_DEMO_STEREO_CALIB_DEMO_H_
#define CORE_DEMO_STEREO_CALIB_DEMO_H_

#include <memory>
#include "opencv2/opencv.hpp"

#include "core/camera_calibration.h"
#include "core/caroline.h"
#include "json/value.h"

namespace base {

class CommandLine;

}  // namespace base

namespace demo {

class StereoCalibDemo : public core::Caroline {
 public:
  explicit StereoCalibDemo(base::CommandLine* command_line);

  virtual bool Init() override;

  virtual int Run() override;

  static const char kDemoName[];

 protected:
  Json::Value MatxToJSON(const cv::Mat& matx);

 private:
  core::CameraCalibration calib_;

  cv::VideoCapture cap1_;
  cv::VideoCapture cap2_;

  int cap_number1_;
  int cap_number2_;

  cv::Mat frame1_;
  cv::Mat frame2_;
};

}  // namespace demo

#endif  // CORE_DEMO_STEREO_CALIB_DEMO_H_
