// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author:

#ifndef CORE_CAMERA_CALIBRATION_H_
#define CORE_CAMERA_CALIBRATION_H_

#include <string>
#include <vector>
#include "opencv2/opencv.hpp"


namespace core {

class Cameras;

class CameraCalibration {
 public:
    CameraCalibration() {}

    void addImagePair(const cv::Mat &image1, const cv::Mat &image2);//two image
    Cameras calibrate(int nx, int ny, float square_size);
 private:
    std::vector<cv::Mat> left_images_;
    std::vector<cv::Mat> right_images_;
};

}  // namespace core

#endif  // CORE_CAMERA_CALIBRATION_H_

