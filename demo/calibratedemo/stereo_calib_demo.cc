// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author: Kukleva Anna <Kuklevaanna@gmail.com>

#include "demo/calibratedemo/stereo_calib_demo.h"

namespace {

const char kCapNumberNode[] = "cap_number";
const char kFlowDemoNode[] = "demo_flowdemo";
}

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

    cv::moveWindow("left", 0, 0);
    cv::moveWindow("right", 500, 0);

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

  base::DictionaryValue *json_K1 = MatxToJSON(cv::Mat(cam.K1()));
  base::DictionaryValue *json_K2 = MatxToJSON(cv::Mat(cam.K2()));
  base::DictionaryValue *json_P1 = MatxToJSON(cv::Mat(cam.P1()));
  base::DictionaryValue *json_P2 = MatxToJSON(cv::Mat(cam.P2()));
  base::DictionaryValue *json_D1 = MatxToJSON(cam.D1());
  base::DictionaryValue *json_D2 = MatxToJSON(cam.D2());
  base::DictionaryValue *json_R = MatxToJSON(cv::Mat(cam.R()));
  base::DictionaryValue *json_T = MatxToJSON(cv::Mat(cam.T()));

  core::Config config;

  config.dictionary()->InsertValue("K1", json_K1);
  config.dictionary()->InsertValue("K2", json_K2);
  config.dictionary()->InsertValue("P1", json_P1);
  config.dictionary()->InsertValue("P2", json_P2);
  config.dictionary()->InsertValue("D1", json_D1);
  config.dictionary()->InsertValue("D2", json_D2);
  config.dictionary()->InsertValue("R", json_R);
  config.dictionary()->InsertValue("T", json_T);

  config.SaveToFile("../im/calib_param");

  return core::RETURN_OK;
}

base::DictionaryValue *StereoCalibDemo::MatxToJSON(const cv::Mat& matx) {
  base::DictionaryValue *json_matrix = new base::DictionaryValue( );
  base::ListValue *json_matrix_data = new base::ListValue( );

  for (int i = 0; i < matx.size().height; i++) {
    for (int j = 0; j < matx.size().width; j++) {
      json_matrix_data->AddValue(
        new base::FloatValue( matx.at<double>(j, i)));
    }
  }

  json_matrix->InsertValue("data", json_matrix_data);
  json_matrix->InsertValue(
        "width", new base::IntegerValue(matx.size().width));
  json_matrix->InsertValue(
        "height", new base::IntegerValue(matx.size().height));

  return json_matrix;
}

}  // namespace demo
