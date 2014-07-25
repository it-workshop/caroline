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

    enum CameraOrientation{
      kLeft,
      kRight
    };

    void addImagePair(const cv::Mat &image1,
    const cv::Mat &image2, int nx, int ny);  //  two image
    Cameras calibrate(int nx, int ny, float square_size);
    cv::Matx33d CalebrationOneCamera(int CameraOrientation,
                                     int nx, int ny, float square_size);
    void HarvestChessboardIdealPointList(int nx, int ny,
                                         float square_size);

    void set_first(const cv::Matx33d& first) { first_ = first; }
    void set_second(const cv::Matx33d& second) { second_ = second; }

    cv::Matx33d first() const { return first_; }
    cv::Matx33d second() const { return second_; }

 private:
    std::vector<cv::Mat> left_images_;
    std::vector<cv::Mat> right_images_;
    std::vector<cv::Point3f> object_points_;
    std::vector<std::vector<cv::Point2f>> VPoints1;
    std::vector<std::vector<cv::Point2f>> VPoints2;
    std::vector<double> D;
    cv::Matx33d first_;
    cv::Matx33d second_;
};

}  // namespace core

#endif  // CORE_CAMERA_CALIBRATION_H_

