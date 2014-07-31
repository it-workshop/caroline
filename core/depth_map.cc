// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "core/depth_map.h"

#include <utility>

#include "core/cameras.h"
#include "core/triangulation.h"


namespace core {

DepthMap::DepthMap(int width, int height) {
  width_ = width;
  height_ = height;


  depth_map_.resize(width_ * height_);

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      depth_map_[width_ * j + i] = kBadDepth;
    }
  }
}

void DepthMap::SetDepth(int x, int y, double depth) {
  if (x < 0 || y < 0) {
    return;
  }

  if ((x > width_) || (y > height_)) {
    return;
  }

  depth_map_[width_ * y + x] = depth;
}

double DepthMap::Depth(int x, int y) const {
  if (x < 0 || y < 0) {
    return kBadDepth;
  }

  if ((x > width_) || (y > height_)) {
    return kBadDepth;
  }

  return depth_map_[width_ * y + x];
}

std::unique_ptr<DepthMap> DepthMap::BuildMapTriangulate(
    const std::vector<std::pair<cv::Point2d, cv::Point2d> > &flow,
    const Cameras &cam, int w, int h) {
  if ((w * h) != flow.size()) {
    return nullptr;
  }

  std::unique_ptr<DepthMap> map(new DepthMap(w, h));
  Triangulation triangulator;
  triangulator.SetCameraMatrices(cam);


  for (size_t i = 0; i < flow.size(); i++) {
    cv::Point2d p = flow.at(i).first;

    double depth = triangulator.TriangulateDepth(p, flow.at(i).second);

    map->SetDepth(static_cast<int>(p.x), static_cast<int>(p.y), depth);
  }

  return map;
}

std::unique_ptr<DepthMap> DepthMap::BuildMapStereoMatching(
    const Cameras& cam,
    const cv::Mat& im_one,
    const cv::Mat& im_two) {

  if (im_one.size != im_two.size) {
    return nullptr;
  }

  cv::Mat R1;
  cv::Mat P1;
  cv::Mat R2;
  cv::Mat P2;
  cv::Mat Q;

  cv::stereoRectify(cam.K1(), cam.D1(), cam.K2(),
                    cam.D2(), im_one.size(), cam.R(),
                    cam.T(), R1, R2, P1, P2, Q);

  cv::Mat map11;
  cv::Mat map12;
  cv::Mat map21;
  cv::Mat map22;

  cv::initUndistortRectifyMap(cam.K1(), cam.D1(), R1, P1,
                              im_one.size(), CV_16SC2, map11, map12);
  cv::initUndistortRectifyMap(cam.K2(), cam.D2(), R2, P2,
                              im_two.size(), CV_16SC2, map21, map22);

  cv::Mat im_one_r;
  cv::Mat im_two_r;

  cv::remap(im_one, im_one_r, map11, map12, cv::INTER_LINEAR);
  cv::remap(im_two, im_two_r, map21, map22, cv::INTER_LINEAR);

  int num_of_disparities = ((im_one.size().width / 8) + 15) & -16;
  int sgbm_win_size = 3;
  int cn = im_one.channels();

  cv::Ptr<cv::StereoSGBM> sgbm = cv::createStereoSGBM(0, 16, 3);
  sgbm->setP1(8 * cn * sgbm_win_size * sgbm_win_size);
  sgbm->setP2(32 * cn * sgbm_win_size * sgbm_win_size);
  sgbm->setMinDisparity(0);
  sgbm->setNumDisparities(num_of_disparities);
  sgbm->setUniquenessRatio(10);
  sgbm->setSpeckleWindowSize(100);
  sgbm->setSpeckleRange(32);
  sgbm->setDisp12MaxDiff(1);
  sgbm->setMode(cv::StereoSGBM::MODE_SGBM);

  cv::Mat disp;
  sgbm->compute(im_one_r, im_two_r, disp);

  cv::Mat disp8;
  disp.convertTo(disp8, CV_8U);

  cv::imwrite("/home/rayman/Downloads/rect.jpg", disp8);
}

}  // namespace core
