// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Sirotkin Dmitry <dmitriy.v.sirotkin@gmail.com
#include <memory>
#include "gtest/gtest.h"
#include "core/depth_map.h"
#include "core/median_map_filter.h"

TEST(MapFilterTest, TestNum) {
  int h = 5;
  int w = 5;
  core::DepthMap maps(h, w);

  for (int i = 0; i < h; ++i)
       for (int j = 0; j < w; ++j)
         maps.SetDepth(j, i, 2);

  maps.SetDepth(2, 2, 1);

  core::MedianMapFilter filterr;
  filterr.SetKernel(3);

  core::DepthMap new_map = filterr.filter(maps);

  ASSERT_EQ(new_map.Depth(2, 2), 2);
  ASSERT_EQ(new_map.Depth(0, 0), 2);
  ASSERT_EQ(new_map.Depth(1, 1), 2);
}

TEST(MapFilterTest1, TestNum1) {
  int h = 3;
  int w = 7;
  core::DepthMap maps(h, w);

  for (int i = 0; i < h; ++i)
       for (int j = 0; j < w; ++j)
         maps.SetDepth(j, i, 0);

  maps.SetDepth(0, 0, 0);
  maps.SetDepth(0, 1, 5);
  maps.SetDepth(0, 2, -1);
  maps.SetDepth(1, 0, 0);
  maps.SetDepth(1, 1, 4);
  maps.SetDepth(1, 2, -2);
  maps.SetDepth(2, 0, 0);
  maps.SetDepth(2, 1, 6);
  maps.SetDepth(2, 2, -3);
  maps.SetDepth(3, 0, 1);
  maps.SetDepth(3, 1, 1);
  maps.SetDepth(3, 2, 1);
  maps.SetDepth(4, 0, 5);
  maps.SetDepth(4, 1, 4);
  maps.SetDepth(4, 2, 2);
  maps.SetDepth(5, 0, 1);
  maps.SetDepth(5, 1, 2);
  maps.SetDepth(5, 2, 3);
  maps.SetDepth(6, 0, 0);
  maps.SetDepth(6, 1, 0);
  maps.SetDepth(6, 2, 0);

  core::MedianMapFilter filterr;
  filterr.SetKernel(3);

  core::DepthMap new_map = filterr.filter(maps);
  ASSERT_EQ(new_map.Depth(1, 1), 0);
  ASSERT_EQ(new_map.Depth(2, 1), 1);
  ASSERT_EQ(new_map.Depth(3, 1), 1);
  ASSERT_EQ(new_map.Depth(4, 1), 2);
  ASSERT_EQ(new_map.Depth(5, 1), 2);
  }

TEST(MapFilterTest2, TestNum2) {
  int h = 5;
  int w = 5;
  core::DepthMap maps(h, w);

  for (int i = 0; i < h; ++i)
       for (int j = 0; j < w; ++j)
         maps.SetDepth(j, i, 0);

  maps.SetDepth(0, 0, 1);
  maps.SetDepth(0, 1, 0);
  maps.SetDepth(0, 2, 1);
  maps.SetDepth(0, 3, 0);
  maps.SetDepth(0, 4, 1);
  maps.SetDepth(1, 0, 0);
  maps.SetDepth(1, 1, 1);
  maps.SetDepth(1, 2, 0);
  maps.SetDepth(1, 3, 1);
  maps.SetDepth(1, 4, 0);
  maps.SetDepth(2, 0, 1);
  maps.SetDepth(2, 1, 0);
  maps.SetDepth(2, 2, 1);
  maps.SetDepth(2, 3, 0);
  maps.SetDepth(2, 4, 1);
  maps.SetDepth(3, 0, 0);
  maps.SetDepth(3, 1, 1);
  maps.SetDepth(3, 2, 0);
  maps.SetDepth(3, 3, 1);
  maps.SetDepth(3, 4, 0);
  maps.SetDepth(4, 0, 1);
  maps.SetDepth(4, 1, 0);
  maps.SetDepth(4, 2, 1);
  maps.SetDepth(4, 3, 0);
  maps.SetDepth(4, 4, 1);

  core::MedianMapFilter filterr;
  filterr.SetKernel(5);

  core::DepthMap new_map = filterr.filter(maps);

  ASSERT_EQ(new_map.Depth(0, 0), 1);
  ASSERT_EQ(new_map.Depth(2, 1), 1);
  ASSERT_EQ(new_map.Depth(3, 2), 1);
  ASSERT_EQ(new_map.Depth(4, 4), 1);
}
