// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author:

#include <memory>
#include <string>
#include "gtest/gtest.h"
#include "camera_calibration.h"
#include "cameras.h"

TEST( CamerasCalibrateTest, Test1) {
  core::Cameras cam;
  core::CameraCalibration calib;

  std::string str = "0";
  std::string str2 = "0";
  int j = 0;

  for( int i = 0; i<21; i++) {
   if(j>=10) str2 ="";
   cv::Mat left_im = cv::imread ("/Users/annushka/imagenes/left"+ str2 + str +".ppm", cv::IMREAD_GRAYSCALE);
   cv::Mat right_im = cv::imread( "/Users/annushka/imagenes/right" + str2 + str +".ppm", cv::IMREAD_GRAYSCALE);
   str = std::to_string(i+1);
   j ++;
   calib.addImagePair(left_im, right_im);
  }


   cam = calib.calibrate( 9, 6, 2.8);
   for(int i = 0; i < 3; i++ ){
     for( int j = 0; j < 4; j++){
       std::cout << cam.P2()(i, j) << " ";  // you can chooce matrix, which you want to see

    }
    std::cout << "\n";
  }
}
