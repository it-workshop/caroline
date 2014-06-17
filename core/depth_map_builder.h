// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_DEPTH_MAP_BUILDER_H_
#define CORE_DEPTH_MAP_BUILDER_H_

#include "core/cameras.h"
#include "core/depth_map.h"
#include "core/optical_flow.h"

namespace core {

class DepthMapBuilder {
 public:
  static DepthMap *BuildMap(const OpticalFlow& flow,
                            const Cameras& cam,
                            int w, int h);
};

}  // namespace core

#endif  // CORE_DEPTH_MAP_BUILDER_H_
