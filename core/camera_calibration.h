// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Kukleva Anna <Kuklevaanna@gmail.com>

#ifndef CORE_CAMERA_CALIBRATION_H_
#define CORE_CAMERA_CALIBRATION_H_

#include <string>
#include <vector>
#include "opencv2/opencv.hpp"

namespace core {

class Cameras;

///

class CameraCalibration {
 public:

  /// Constructor.
    CameraCalibration() {}
  /// Type of camera position
    enum CameraOrientation {
      /// Left position
      kLeft,
      /// Right position
      kRight
    };

    /// Checks pairs of images and adds their into vectors if number of corners
    /// on the images match with right number of corners.
    /// Refines the corner locations and adds their coordinates into vectors
    /// @param[in] image1 Left image
    /// @param[in] image2 Right image
    /// @param[in] nx Number of internal corners horizontally
    /// @param[in] ny Number of internal corners vertically
    void addImagePair(const cv::Mat &image1,
                      const cv::Mat &image2,
                      int nx, int ny);
    /// Calibrates two cameras
    /// @param[in] nx Number of internal corners horizontally
    /// @param[in] ny Number of internal corners vertically
    /// @param[in] squre_size Size of square on the chessboard in cm
    /// @returns 2 matrix internal parameters of each camera and 2 rotation matrix
    /// between the 1st and the 2nd camera coordinate systems with
    /// translation vector between the coordinate systems of the cameras
    Cameras calibrate(int nx, int ny, float square_size);
    /// Calibrates one camera
    /// @param[in] CameraOrientation Сamera Position
    /// @param[in] nx Number of internal corners horizontally
    /// @param[in] ny Number of internal corners vertically
    /// @param[in] squre_size Size of square on the chessboard in cm
    /// @returns matrix internal parameters
    cv::Matx33d CalebrationOneCamera(int CameraOrientation,
                                     int nx, int ny, float square_size);
    /// Records coordinates pattern into vector
    /// @param[in] nx Number of internal corners horizontally
    /// @param[in] ny Number of internal corners vertically
    /// @param[in] squre_size Size of square on the chessboard in cm
    void HarvestChessboardIdealPointList(int nx, int ny,
                                         float square_size);

    /// Records matrix in private variable
    /// @param[in] first Matrix 3x3
    void set_first(const cv::Matx33d& first) { first_ = first; }
    /// Records matrix in private variable
    /// @param[in] second Matrix 3x3
    void set_second(const cv::Matx33d& second) { second_ = second; }

    /// @returns recorded matrix
    cv::Matx33d first() const { return first_; }
    /// @returns recorded matrix
    cv::Matx33d second() const { return second_; }

 private:
    /// Vector of left images
    std::vector<cv::Mat> left_images_;
    /// Vector of right images
    std::vector<cv::Mat> right_images_;
    /// Vector of pattern coordinates ....
    std::vector<cv::Point3f> object_points_;
    /// Vector of corners coordinate of left images
    std::vector<std::vector<cv::Point2f>> VPoints1;
    /// Vector of corners coordinate of left images
    std::vector<std::vector<cv::Point2f>> VPoints2;
    /// Vector of distortion coefficients
    std::vector<double> D;
    /// Matrix of internal paramaters left camera
    cv::Matx33d first_;
    /// Matrix of internal paramaters right camera
    cv::Matx33d second_;
};

}  // namespace core

#endif  // CORE_CAMERA_CALIBRATION_H_

