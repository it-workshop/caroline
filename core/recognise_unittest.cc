// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Sirotkin Dmitry <dmitriy.v.sirotkin@gmail.com
#include <memory>
#include <iostream>
#include "gtest/gtest.h"
#include "core/depth_map.h"
#include "core/map_recognise.h"


TEST(MapRecogniseTest, TestNumber) {
  int h = 10;
  int w = 10;
  core::DepthMap maps(h, w);

  for (int i = 0; i < h; ++i)
       for (int j = 0; j < w; ++j)
         maps.SetDepth(i, j, 1);

  maps.SetDepth(0, 0, 1);
  maps.SetDepth(0, 1, 3);
  maps.SetDepth(0, 2, 1);
  maps.SetDepth(1, 0, 3);
  maps.SetDepth(1, 1, 7);
  maps.SetDepth(1, 2, 1);
  maps.SetDepth(2, 0, 1);
  maps.SetDepth(2, 1, 1);
  maps.SetDepth(2, 2, 1);


  core::MapRecognise recognise;
  recognise.SetPrecision(2);

  core::DepthMap object_map = recognise.filter(maps);

  ASSERT_EQ(1, object_map.Depth(0, 0));
  ASSERT_EQ(2, object_map.Depth(0, 1));
  ASSERT_EQ(3, object_map.Depth(7, 7));
  ASSERT_EQ(4, object_map.Depth(1, 0));
  ASSERT_EQ(5, object_map.Depth(1, 1));
}
