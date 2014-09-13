// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "core/surface_mesh.h"

namespace core {

SurfaceMesh::SurfaceMesh(const PointCloud &cloud) {
  SetVertexes(cloud.Points());
}

void SurfaceMesh::FillFaces() {
  // Any algo for polygonize. Fill faces vector.
}

}  // namespace core
