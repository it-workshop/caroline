// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_SURFACE_MESH_H_
#define CORE_SURFACE_MESH_H_

#include "core/mesh.h"
#include "core/point_cloud.h"

namespace core {

class SurfaceMesh : public Mesh {
 public:
  explicit SurfaceMesh(const PointCloud& cloud);

  void FillFaces();
};

}  // namespace core

#endif  // CORE_SURFACE_MESH_H_
