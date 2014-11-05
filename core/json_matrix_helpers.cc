// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Mlodik Mikhail <mlodik_m@mail.ru>

#include "core/json_matrix_helpers.h"

#include "json/value.h"

namespace core {

namespace {

const char kWidthNode[] = "width";
const char kHeightNode[] = "height";
const char kDataNode[] = "data";

/// Get width of matrix.
/// @param [in] json
/// @returns width of given matrix.
size_t MatrixWidth(const Json::Value& json) {
  return json[kWidthNode].asUInt();
}

/// Get height of matrix.
/// @param [in] json
/// @returns height of given matrix.
size_t MatrixHeight(const Json::Value& json) {
  return json[kHeightNode].asUInt();
}

/// Checks that json is a valid saved matrix.
/// @param [in] json Json value to check.
/// @returns true if matrix can be extracted.
bool IsJsonMatrix(const Json::Value& json) {
  return json.isObject() &&
      json.isMember(kWidthNode) &&
      json[kWidthNode].isUInt() &&
      json.isMember(kHeightNode) &&
      json[kHeightNode].isUInt() &&
      json.isMember(kDataNode) &&
      json[kDataNode].isArray() &&
      MatrixWidth(json) * MatrixHeight(json) == json[kDataNode].size();
}

}  // namespace

bool JsonToMat(const Json::Value& json_matrix, cv::Mat* mat) {
  if (!IsJsonMatrix(json_matrix))
    return false;
  const int w = MatrixWidth(json_matrix);
  const int h = MatrixHeight(json_matrix);
  cv::Mat data(h, w, CV_64F);
  const Json::Value& json_matrix_data = json_matrix[kDataNode];
  for (int i = 0; i < h; ++i)
    for (int j = 0; j < w; ++j) {
      if (!json_matrix_data[i * w + j].isDouble())
        return false;
      data.at<double>(i, j) = json_matrix_data[i * w + j].asDouble();
  }
  *mat = data;
  return true;
}

Json::Value MatToJson(const cv::Mat& mat) {
  Json::Value json_matrix(Json::objectValue);
  Json::Value json_matrix_data(Json::arrayValue);

  for (auto it = mat.begin<double>(), end = mat.end<double>(); it != end; ++it)
    json_matrix_data.append(*it);

  json_matrix[kDataNode] = json_matrix_data;
  json_matrix[kWidthNode] = mat.size().width;
  json_matrix[kHeightNode] = mat.size().height;

  return json_matrix;
}

}  // namespace core

