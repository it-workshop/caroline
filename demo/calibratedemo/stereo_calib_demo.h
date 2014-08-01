// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Kukleva Anna <Kuklevaanna@gmail.com>


#ifndef DEMO_CALIBRATEDEMO_STEREO_CALIB_DEMO_H_
#define DEMO_CALIBRATEDEMO_STEREO_CALIB_DEMO_H_

#include <memory>
#include "opencv2/opencv.hpp"

#include "core/camera_calibration.h"
#include "base/values.h"
#include "demo/calibratedemo/stereo_calib_demo.h"
#include "core/config.h"
#include "core/image_capture_manager.h"
#include "core/time_controller.h"
#include "core/cameras.h"
#include "core/return_codes.h"

namespace base {

class CommandLine;

}  // namrspace base

namespace demo {

class StereoCalibDemo {
 public:
  StereoCalibDemo();

  bool Init();

  int Run();

 protected:
  base::DictionaryValue *MatxToJSON(const cv::Mat& matx);

 private:
  core::CameraCalibration calib_;

  std::unique_ptr<core::ImageCaptureManager> image_capture_manager_;

  cv::VideoCapture cap1_;
  cv::VideoCapture cap2_;

  int cap_number1_;
  int cap_number2_;

  cv::Mat frame1_;
  cv::Mat frame2_;
};

}  // namespace demo

#endif  // DEMO_CALIBRATEDEMO_STEREO_CALIB_DEMO_H_
