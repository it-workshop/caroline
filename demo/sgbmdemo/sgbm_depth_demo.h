// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef DEMO_SGBMDEMO_SGBM_DEPTH_DEMO_H_
#define DEMO_SGBMDEMO_SGBM_DEPTH_DEMO_H_

#include <memory>

#include "opencv2/core/mat.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

namespace core {
  class Cameras;
}

namespace demo {

class SGBMDepthDemo {
 public:
  SGBMDepthDemo();
  virtual ~SGBMDepthDemo() {}

  bool Init();

  int Run();

 protected:
  void FillCams();

 private:
  cv::VideoCapture cap1_;
  cv::VideoCapture cap2_;

  cv::Mat frame1_;
  cv::Mat frame2_;

  std::unique_ptr<core::Cameras> cams_;
};

}  // namespace demo

#endif  // DEMO_SGBMDEMO_SGBM_DEPTH_DEMO_H_
