// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#include <memory>

#include "gtest/gtest.h"
#include "opencv2/opencv.hpp"

// This file is needed to make sure that opencv is linked correctly.
TEST(OpenCVTest, MatTest) {
  EXPECT_NE(nullptr, std::unique_ptr<cv::Mat>(new cv::Mat()));
}
