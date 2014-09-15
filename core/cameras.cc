// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "core/cameras.h"

#include "core/quaternion.h"

namespace core {

Cameras::Cameras() {
}

cv::Matx33d Cameras::CameraMatrix(int dpm, double focus_length, int w, int h) {
  cv::Matx33d K(dpm * focus_length, 0, w / 2,
                0, dpm * focus_length, h / 2,
                0, 0, 1);

  return K;
}

cv::Matx34d Cameras::ProjectiveMatrix(const Quaternion& quat,
                                      const cv::Point3d& pos) {
  double w = quat.w();
  double x = quat.x();
  double y = quat.y();
  double z = quat.z();

  return cv::Matx34d(
    1 - 2 * (y * y + z * z), 2 * (x * y - z * w), 2 * (x * z + y * w), pos.x,
    2 * (x * y + z * w), 1 - 2 * (x * x + z * z), 2 * (y * z - x * w), pos.y,
    2 * (x * z - y * w), 2 * (y * z + x * w), 1 - 2 * (x * x + y * y), pos.z);
}

}  // namespace core
