// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_STAT_UTILS_H_
#define CORE_STAT_UTILS_H_

#include <cstddef>  // size_t

#include "base/logging.h"
#include "opencv2/core/mat.hpp"

namespace core {

namespace stat {

/// Generic algorithms for reduce any cv::Mat.
/// @param[in] src Input matrix of any type.
/// @param[in] init Initial value of the reduction of the same type as src.
/// @param[in] func Binary functor,
/// which returns the value of the same type as init.
///
/// @returns init value, after reduction the src
template<typename T, typename Func, typename Mat>
T reduce(const Mat& src, T init, Func func) {
  for (size_t row = 0; row < src.rows; row++) {
    auto src_ptr = src.template ptr<T>(row);
    for (size_t col = 0; col < src.cols; col++) {
      init = func(init, src_ptr[col]);
    }
  }

  return init;
}

/// Generic algorithms for sapply any cv::Mat.
/// @param[in] src Input matrix of any type.
/// @param[out] dst Output matrix of the same type and size as src.
/// @param[in] func Unary functor,
/// which is applied to each element of the matrix src.
template<typename T, typename Func, typename Mat>
void sapply(const Mat& src, Mat* dst, Func func) {
  if (src.size != dst->size) {
    LOG(WARNING) << "Size of the src matrix != size of the dst matrix.";
    return;
  }

  for (size_t row = 0; row < src.rows; row++) {
    auto src_ptr = src.template ptr<T>(row);
    auto dst_ptr = dst->template ptr<T>(row);
    for (size_t col = 0; col < src.cols; col++) {
      dst_ptr[col] = func(src_ptr[col]);
    }
  }
}

}  // namespace stat

}  // namespace core


#endif  // CORE_STAT_UTILS_H_
