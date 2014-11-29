// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#include "core/json_matrix_helpers.h"
#include "gtest/gtest.h"

template <class T>
struct Wrapper {
  explicit Wrapper(const T& t)
    : object(t) {}
  bool operator==(const Wrapper<T>& that) const {
    return object == that.object;
  }
  T object;
};

template <class T>
Wrapper<T> Wrap(const T& t) { return Wrapper<T>(t); }

TEST(JsonMatrixHelpersTest, MatxToJson) {
  cv::Matx34d mat = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
  Json::Value value = core::MatxToJson(mat);
  cv::Matx34d mat_2;
  EXPECT_TRUE(core::JsonToMatx(value, &mat_2));
  EXPECT_EQ(Wrap(mat), Wrap(mat_2));
}

