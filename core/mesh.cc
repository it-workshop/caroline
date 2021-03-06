// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "core/mesh.h"

namespace core {

Mesh::Mesh() {
}

void Mesh::AddVertex(const cv::Point3d &point) {
  vertexes_.push_back(point);
}

void Mesh::AddFace(const Triangle &face) {
  faces_.push_back(face);
}

void Mesh::SetVertexes(const std::vector<cv::Point3d>& vertexes) {
  vertexes_ = vertexes;
}

void Mesh::SetFaces(const std::vector<Triangle>& faces) {
  faces_ = faces;
}

}  // namespace core
