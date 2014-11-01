// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Mlodik Mikhail <mlodik_m@mail.ru>

#ifndef CORE_JSON_MATRIX_HELPERS_H_
#define CORE_JSON_MATRIX_HELPERS_H_

#include "json/value.h"
#include "opencv2/core/core.hpp"

namespace core {

/// Read values from json and fill matrix.
/// @param [in] json_matrix Json value to read.
/// @param [out] mat Matrix object to fill.
/// @returs true on success
bool JsonToMat(const Json::Value& json_matrix, cv::Mat* mat);

/// Read values from json and fill matrix m x n with double.
/// @param [in] json_matrix Json value to read.
/// @param [out] mat Matrix object to fill.
/// @returs true on success
template <int m, int n>
bool JsonToMatx(const Json::Value& json_matrix,
                   cv::Matx<double, m, n>* mat) {
  cv::Mat temp_mat;
  if (!JsonToMat(json_matrix, &temp_mat) ||
      temp_mat.size().width != n ||
      temp_mat.size().height != m)
    return false;
  *mat = temp_mat;
  return true;
}

/// Convert matrix into json representation.
/// @param [in] mat Matrix to convert.
/// @returns Corespound json value.
Json::Value MatToJson(const cv::Mat& mat);

/// Convert matrix m x n of double into json representation.
/// @param [in] mat Matrix to convert.
/// @returns Corespound json value.
template <int m, int n>
Json::Value MatxToJson(const cv::Matx<double, m, n>& mat) {
  return MatToJson(cv::Mat(mat));
}

}  // namespace core

#endif  // CORE_JSON_MATRIX_HELPERS_H_

