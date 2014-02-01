// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "core/mesh.h"

namespace core {

Mesh::Mesh() {
}

void Mesh::AddVertex(const Point3D &point) {
  vertexes_.push_back(point);
}

void Mesh::AddFace(const Triangle &face) {
  faces_.push_back(face);
}

void Mesh::SetVertexes(const std::vector<Point3D> vertexes) {
  vertexes_ = vertexes;
}

void Mesh::SetFaces(const std::vector<Triangle> faces) {
  faces_ = faces;
}

}  // namespace core
