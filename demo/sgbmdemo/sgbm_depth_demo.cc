// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "demo/sgbmdemo/sgbm_depth_demo.h"

#include "core/cameras.h"
#include "core/depth_map.h"

namespace demo {

SGBMDepthDemo::SGBMDepthDemo() {
}

bool SGBMDepthDemo::Init() {
  cap1_ = cv::VideoCapture(0);
  cap2_ = cv::VideoCapture(1);

  FillCams();

  return cap1_.isOpened() && cap2_.isOpened();
}

int SGBMDepthDemo::Run() {
  cv::namedWindow("disp", 1);

  for ( ; ; ) {
    cap1_ >> frame1_;
    cap2_ >> frame2_;

    std::unique_ptr<core::DepthMap> map =
        core::DepthMap::BuildMapStereoMatching(*cams_, frame1_, frame2_);

    cv::Mat disp(map->width(), map->height(), CV_32F);
    for (int i = 0; i < map->width(); i++) {
      for (int j = 0; j < map->height(); j++) {
        disp.at<float>(i, j) = map->Depth(i, j);
      }
    }

    cv::imshow("disp", disp);

    cv::waitKey(100);
  }
}

void SGBMDepthDemo::FillCams() {

}

}  // namespace demo
