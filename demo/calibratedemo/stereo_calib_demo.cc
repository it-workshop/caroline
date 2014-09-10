// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author: Kukleva Anna <Kuklevaanna@gmail.com>

#include "demo/calibratedemo/stereo_calib_demo.h"

namespace {

const char kCapNumberNode[] = "cap_number";
const char kFlowDemoNode[] = "demo_flowdemo";
}

#if CV_MAJOR > 2 || (CV_MAJOR == 2 && CV_MINOR >= 4)
#define cvMoveWindow cv::moveWindow
#endif

namespace demo {

const int x = 7;
const int y = 5;
float sq_size = 30;

StereoCalibDemo::StereoCalibDemo()
  : cap_number1_(2),
    cap_number2_(1) {
}

bool StereoCalibDemo::Init() {
  cap1_ = cv::VideoCapture(cap_number1_);
  cap2_ = cv::VideoCapture(cap_number2_);

  return cap1_.isOpened() && cap2_.isOpened();
}

int StereoCalibDemo::Run() {
  core::Cameras cam;
  core::CameraCalibration calib;

  cv::Matx33d intr_mat;

  // Number of pair images
  int k = 3;

  for (int i = 0; i < k; i++) {
    cap1_ >> frame1_;
    cap2_ >> frame2_;

    cv::cvtColor(frame1_, frame1_, cv::COLOR_BGR2GRAY);
    cv::cvtColor(frame2_, frame2_, cv::COLOR_BGR2GRAY);

    calib.addImagePair(frame1_, frame2_, x, y);

    cv::imwrite("../im/left" + std::to_string(i) + ".png", frame1_);
    cv::imwrite("../im/right" + std::to_string(i) + ".png", frame2_);

    cv::resize(frame1_, frame1_, cv::Size(),
               0.3, 0.3);
    cv::resize(frame2_, frame2_, cv::Size(),
               0.3, 0.3);

    cv::imshow("left", frame1_);
    cv::imshow("right", frame2_);

    cvMoveWindow("left", 0, 0);
    cvMoveWindow("right", 500, 0);

    cv::waitKey(-1);
  }

  cv::destroyWindow("left");
  cv::destroyWindow("right");

  if (!calib.return_size()) {
    std::cout << "No chessboard on images.\n";
    return core::RETURN_OK;
  }

  calib.HarvestChessboardIdealPointList(x, y, sq_size);

  cam = calib.calibrate(x, y, sq_size);

  Json::Value json_K1 = MatxToJSON(cv::Mat(cam.K1()));
  Json::Value json_K2 = MatxToJSON(cv::Mat(cam.K2()));
  Json::Value json_P1 = MatxToJSON(cv::Mat(cam.P1()));
  Json::Value json_P2 = MatxToJSON(cv::Mat(cam.P2()));
  Json::Value json_D1 = MatxToJSON(cam.D1());
  Json::Value json_D2 = MatxToJSON(cam.D2());
  Json::Value json_R = MatxToJSON(cv::Mat(cam.R()));
  Json::Value json_T = MatxToJSON(cv::Mat(cam.T()));

  core::Config config;

  (*config.dictionary())["K1"] = json_K1;
  (*config.dictionary())["K2"] = json_K2;
  (*config.dictionary())["P1"] = json_P1;
  (*config.dictionary())["P2"] = json_P2;
  (*config.dictionary())["D1"] = json_D1;
  (*config.dictionary())["D2"] = json_D2;
  (*config.dictionary())["R"] = json_R;
  (*config.dictionary())["T"] = json_T;

  config.SaveToFile("../im/calib_param");

  return core::RETURN_OK;
}

Json::Value StereoCalibDemo::MatxToJSON(const cv::Mat& matx) {
  Json::Value json_matrix(Json::objectValue);
  Json::Value json_matrix_data(Json::arrayValue);

  for (int i = 0; i < matx.size().height; i++)
    for (int j = 0; j < matx.size().width; j++)
      json_matrix_data.append(matx.at<double>(j, i));

  json_matrix["data"] = json_matrix_data;
  json_matrix["width"] = matx.size().width;
  json_matrix["height"] = matx.size().height;

  return json_matrix;
}

}  // namespace demo
