// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "core/cameras.h"

#include <iostream>

#include "core/config.h"
#include "core/json_matrix_helpers.h"
#include "core/quaternion.h"

namespace core {

namespace {

const char kCamerasNode[] = "cameras";
const char kK1Node[] = "k1";
const char kK2Node[] = "k2";
const char kP1Node[] = "p1";
const char kP2Node[] = "p2";
const char kD1Node[] = "d1";
const char kD2Node[] = "d2";
const char kRNode[] = "r";
const char kTNode[] = "t";

}  // namespace

Cameras::Cameras() {
}

// static
cv::Matx33d Cameras::CameraMatrix(int dpm, double focus_length, int w, int h) {
  cv::Matx33d K(dpm * focus_length, 0, w / 2,
                0, dpm * focus_length, h / 2,
                0, 0, 1);

  return K;
}

// static
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

bool Cameras::LoadFromConfig(Config* config) {
  if (!config || !config->dictionary() ||
      !config->dictionary()->isMember(kCamerasNode))
    return false;

  const Json::Value& properties = (*config->dictionary())[kCamerasNode];
  if (!properties.isObject())
    return false;

  cv::Matx33d k1;
  cv::Matx33d k2;
  cv::Matx34d p1;
  cv::Matx34d p2;
  cv::Mat d1;
  cv::Mat d2;
  cv::Matx33d r;
  cv::Matx31d t;
  if (!properties.isMember(kK1Node) ||
      !properties.isMember(kK2Node) ||
      !properties.isMember(kP1Node) ||
      !properties.isMember(kP2Node) ||
      !properties.isMember(kD1Node) ||
      !properties.isMember(kD2Node) ||
      !properties.isMember(kRNode) ||
      !properties.isMember(kTNode) ||
      !JsonToMatx(properties[kK1Node], &k1) ||
      !JsonToMatx(properties[kK2Node], &k2) ||
      !JsonToMatx(properties[kP1Node], &p1) ||
      !JsonToMatx(properties[kP2Node], &p2) ||
      !JsonToMat(properties[kD1Node], &d1) ||
      !JsonToMat(properties[kD2Node], &d2) ||
      !JsonToMatx(properties[kRNode], &r) ||
      !JsonToMatx(properties[kTNode], &t))
    return false;

  set_K1(k1);
  set_K2(k2);
  set_P1(p1);
  set_P2(p2);
  set_D1(d1);
  set_D2(d2);
  set_R(r);
  set_T(t);
  return true;
}

void Cameras::SaveToConfig(Config* config) const {
  Json::Value properties(Json::objectValue);
  properties[kK1Node] = MatxToJson(K1());
  properties[kK2Node] = MatxToJson(K2());
  properties[kP1Node] = MatxToJson(P1());
  properties[kP2Node] = MatxToJson(P2());
  properties[kD1Node] = MatToJson(D1());
  properties[kD2Node] = MatToJson(D2());
  properties[kRNode] = MatxToJson(R());
  properties[kTNode] = MatxToJson(T());
  (*config->dictionary())[kCamerasNode] = properties;
}

}  // namespace core
