// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "stereo_depth_map.h"

#include <iostream>

#include "gtest/gtest.h"

#include "depth_map.h"
#include "depth_mesh.h"
#include "ply_saver.h"

namespace core {

cv::Mat StereoDepthMap::BuildMap(const std::string &leftImage, const std::string &rightImage) {
  cv::StereoSGBM stereo;
  stereo.SADWindowSize = 5;
  stereo.numberOfDisparities = 192;
  stereo.preFilterCap = 4;
  stereo.minDisparity = -64;
  stereo.uniquenessRatio = 1;
  stereo.speckleWindowSize = 150;
  stereo.speckleRange = 2;
  stereo.disp12MaxDiff = 10;
  stereo.fullDP = false;
  stereo.P1 = 8*5*5;
  stereo.P2 = 32*5*5;

  cv::Mat leftIm = cv::imread(leftImage, CV_LOAD_IMAGE_GRAYSCALE);
  cv::Mat rightIm = cv::imread(rightImage, CV_LOAD_IMAGE_GRAYSCALE);

  if ((!leftIm.data) || (!rightIm.data)) {
    return cv::Mat(0,0,CV_8UC1);
  }

  if (leftIm.rows != rightIm.rows)
      return cv::Mat(0,0,CV_8UC1);

  if (leftIm.cols != rightIm.cols)
    return cv::Mat(0,0,CV_8UC1);

  int h = leftIm.rows;
  int w = leftIm.cols;

  cv::Mat disp = cv::Mat(h, w, CV_16S);
  cv::Mat disp8U = cv::Mat(h, w, CV_8UC1 );

  stereo(leftIm, rightIm, disp);

  double minVal; double maxVal;
  minMaxLoc(disp, &minVal, &maxVal );

  disp.convertTo(disp8U, CV_8UC1, 255/(maxVal - minVal));

  cv::imwrite("/home/rayman/Downloads/ALL-2views/Bowling2/depth.png", disp8U);

  minMaxLoc(disp8U, &minVal, &maxVal );
/*
  cv::Mat samples(h * w, 3, CV_32F);
    for( int y = 0; y < h; y++ )
      for( int x = 0; x < w; x++ ) {

          samples.at<float>(y*w + x, 0) = (int)disp8U.at<uchar>(y,x);
          samples.at<float>(y*w + x, 1) = (int)disp8U.at<uchar>(y,x);
          samples.at<float>(y*w + x, 2) = (int)disp8U.at<uchar>(y,x);
      }

  cv::Mat labels;
  cv::Mat centers;

  cv::kmeans(samples, 6, labels, cv::TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 10, 0.01), 10, cv::KMEANS_PP_CENTERS, centers );

  cv::Mat new_image(h, w, CV_8UC1);

  for( int y = 0; y < h; y++ )
    for( int x = 0; x < w; x++ )
    {
      int cluster_idx = labels.at<int>(y*w + x,0);
      new_image.at<uchar>(y,x) = centers.at<float>(cluster_idx, 0);
//      new_image.at<cv::Vec3b>(y,x)[1] = centers.at<float>(cluster_idx, 1);
//      new_image.at<cv::Vec3b>(y,x)[2] = centers.at<float>(cluster_idx, 2);
    }

  cv::imwrite("/home/rayman/Downloads/ALL-2views/Bowling2/claster.png", new_image);
*/
  DepthMap map(h, w);
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
	  map.SetDepth(x, y, 255 - (int)disp8U.at<uchar>(y, x));
    }
  }

  DepthMesh mesh(map, 16, 240);

  PlySaver saver;
  saver.set_path("/home/rayman/Downloads/");
  saver.set_name("mesh2");
  saver.Save(&mesh);


//  return new_image;
  return disp8U;
}

}  // namespace core
