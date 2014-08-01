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

/// Class to calibrate stereo cameras
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
    /// @returns Cameras object for two calibrated cameras
    Cameras calibrate(int nx, int ny, float square_size);
    /// Calibrates one camera
    /// @param[in] CameraOrientation Сamera Position
    /// @param[in] nx Number of internal corners horizontally
    /// @param[in] ny Number of internal corners vertically
    /// @param[in] squre_size Size of square on the chessboard in cm
    /// @returns matrix internal parameters
    void CalibrationOneCamera(int CameraOrientation,
                             int nx, int ny, float square_size,
                             cv::Matx33d* K, std::vector<double>* D);
    /// Records coordinates pattern into vector
    /// @param[in] nx Number of internal corners horizontally
    /// @param[in] ny Number of internal corners vertically
    /// @param[in] squre_size Size of square on the chessboard in cm
    void HarvestChessboardIdealPointList(int nx, int ny,
                                         float square_size);
    /// Returns size
    int return_size() const { return VPoints1.size(); }

 private:
    /// Vector of pattern corners coordinates for each stereo pair
    std::vector<cv::Point3f> object_points_;
    /// Vector of corners coordinate of left images
    std::vector<std::vector<cv::Point2f>> VPoints1;
    /// Vector of corners coordinate of left images
    std::vector<std::vector<cv::Point2f>> VPoints2;
    /// Image size.
    cv::Size image_size_;
};

}  // namespace core

#endif  // CORE_CAMERA_CALIBRATION_H_

