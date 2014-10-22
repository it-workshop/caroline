// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Sirotkin Dmitry <dmitriy.v.sirotkin@gmail.com

#include "core/depth_map.h"
#include "core/map_recognise.h"
#include "core/median_map_filter.h"

namespace core {
MapRecognise::MapRecognise() :
  object_map_(0, 0),
  new_map_(0, 0)
{}

DepthMap MapRecognise::filter(const DepthMap &map) {
  MedianMapFilter myfilter;
  myfilter.SetKernel(1);
  object_map_ = DepthMap(map.width(), map.height());
  new_map_ = DepthMap(map.width(), map.height());
  new_map_ = myfilter.filter(map);
  for (int i = 0; i < map.width(); i++) {
    for (int j = 0; j < map.height(); j++) {
      object_map_.SetDepth(i, j, 0);
      }
  }

  int NumberCounter = 0;
  for (int i = 0; i < map.width(); i++) {
    for (int j = 0; j < map.height(); j++) {
      if (object_map_.Depth(i, j) == 0) {
        ++NumberCounter;
        dfs(i, j, NumberCounter);
      }
    }
  }
  return object_map_;
}

void MapRecognise::SetPrecision(float Precision) {
  precision_ = Precision;  //  Precision>=1
}

int MapRecognise::NeighbourDepth(int i, int j, int direction) {
  if ((direction == 0) && (i > 0))
    return object_map_.Depth(--i, j);
  if ((direction == 1) && (i < (object_map_.width()-1)))
    return object_map_.Depth(++i, j);
  if ((direction == 2) && (j > 0))
    return object_map_.Depth(i, --j);
  if ((direction == 3) && (j < (object_map_.height()-1)))
    return object_map_.Depth(i, ++j);
  return (-1);
}

void MapRecognise::dfs(int i, int j, int NumberCounter) {
  object_map_.SetDepth(i, j, NumberCounter);
    for (int direction = 0; direction < 4; direction++) {
      if ((SmoothNeighbourhood(i, j, direction, precision_) == true) &&
          (NeighbourDepth(i, j, direction) == 0))  {
        if ((direction == 0) && (i > 0)) {
          dfs(--i, j, NumberCounter);
        }
        if ((direction == 1) && (i < (object_map_.width()-1))) {
          dfs(++i, j, NumberCounter);
        }
        if ((direction == 2) && (j > 0)) {
          dfs(i, --j, NumberCounter);
        }
        if ((direction == 3) && (j < (object_map_.height()-1))) {
          dfs(i, ++j, NumberCounter);
        }
      }
    }
}

bool MapRecognise::SmoothNeighbourhood(int w, int h,
                                       int direction, float precision_) {
  float Special;
  if ((direction == 0) && (w > 0)) {
    Special = static_cast<float>
        (new_map_.Depth(w, h) / new_map_.Depth(w - 1, h));
    --w;
  }
  if ((direction == 1) && (w < (new_map_.width() - 1))) {
    Special = static_cast<float>
        (new_map_.Depth(w, h) / new_map_.Depth(w + 1, h));
    ++w;
  }
  if ((direction == 2) && (h > 0)) {
    Special = static_cast<float>
        (new_map_.Depth(w, h) / new_map_.Depth(w, h - 1));
    --h;
  }
  if ((direction == 3) && (h < (new_map_.height() - 1))) {
    Special = static_cast<float>
        (new_map_.Depth(w, h) / new_map_.Depth(w, h + 1));
    ++h;
  }

  return ((Special <= precision_) && (Special >= (1 / precision_)));
}

}  //  namespace core
