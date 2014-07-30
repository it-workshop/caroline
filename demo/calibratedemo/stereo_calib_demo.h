// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Kukleva Anna <Kuklevaanna@gmail.com>


#ifndef DEMO_CALIBRATEDEMO_STEREO_CALIB_DEMO_H_
#define DEMO_CALIBRATEDEMO_STEREO_CALIB_DEMO_H_

namespace core {
  class CameraCalibration;
}

namespace demo {

class StereoCalibDemo {
 public:
  StereoCalibDemo();

 private:
  core::CameraCalibration calib_;

};

}  // namespace demo

#endif  // DEMO_CALIBRATEDEMO_STEREO_CALIB_DEMO_H_
