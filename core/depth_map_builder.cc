// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include <cmath>

#include "core/depth_map_builder.h"
#include "core/triangulation.h"

namespace core {

DepthMap *DepthMapBuilder::BuildMap(const OpticalFlow &flow,
                                    const Cameras &cam,
                                    int w, int h) {
  int size = flow.Size();

  if ((w * h) != size) {
    return nullptr;
  }

  DepthMap *map = new DepthMap(h, w);
  Triangulation triangulator;
  triangulator.SetCameraMatrices(cam);

  for (int i = 0; i < size; i++) {
    cv::Point2d p = flow.ImageOnePoint(i);

    map->SetDepth(p.x, p.y,
                  triangulator.TriangulateDepth(p, flow.ImageTwoPoint(i)));
  }

  return map;
}

}  // namespace core
