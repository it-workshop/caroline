// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "core/depth_mesh.h"

#include <vector>

namespace core {

DepthMesh::DepthMesh(const DepthMap& depth_map,
    const int& min, const int& max) {
  min_ = min;
  max_ = max;

  int height = depth_map.Height();
  int width = depth_map.Width();

  std::vector<std::vector<int> > vertex_counter_matrix;
  vertex_counter_matrix.resize(height);
  for (int i = 0; i < height; i++) {
    vertex_counter_matrix[i].resize(width);
  }

  int counter = 0;

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      vertex_counter_matrix[i][j] = bad_depth;

      int depth = depth_map.Depth(i, j);

      if (IsCorrectDepth(depth)) {
        vertex_counter_matrix[i][j] = counter;
        counter++;

        AddDepthVertex(i, j, depth);
      }
    }
  }

  for (int i = 0; i < height - 1; i++) {
    for (int j = 0; j < width - 1; j++) {
      AddDepthFaces(vertex_counter_matrix[i][j],
          vertex_counter_matrix[i][j + 1],
          vertex_counter_matrix[i + 1][j],
          vertex_counter_matrix[i + 1][j + 1]);
    }
  }
}

void DepthMesh::AddDepthVertex(const int &x,
    const int &y, const double &depth) {
  this->AddVertex(Point3D(x, y, depth));
}

void DepthMesh::AddDepthFace(const int &v1, const int &v2, const int &v3) {
  this->AddFace(Triangle(v1, v2, v3));
}

void DepthMesh::AddDepthFaces(const int &v1,
    const int &v2, const int &v3, const int &v4) {
  FaceType type = Type(v1, v2, v3, v4);

  switch (type) {
  case(ALL_RIGHT) : {
    AddDepthFace(v1, v2, v4);
    AddDepthFace(v1, v3, v4);
    return;
  }
  case(BAD_FACE) : {
    return;
  }
  case(LEFT_TOP) : {
    AddDepthFace(v2, v3, v4);
    return;
  }
  case(RIGHT_TOP) : {
    AddDepthFace(v1, v3, v4);
    return;
  }
  case(LEFT_BOTTOM) : {
    AddDepthFace(v1, v2, v4);
    return;
  }
  case(RIGHT_BOTTOM) : {
    AddDepthFace(v1, v2, v3);
    return;
  }
  }
}

DepthMesh::FaceType DepthMesh::Type(const int &v1,
    const int &v2, const int &v3, const int &v4) {
  FaceType face_type = ALL_RIGHT;

  if (v1 == bad_depth) {
    face_type = LEFT_TOP;
  }

  if (v2 == bad_depth) {
    if (face_type != ALL_RIGHT) {
      return BAD_FACE;
    }

    face_type = RIGHT_TOP;
  }

  if (v3 == bad_depth) {
    if (face_type != ALL_RIGHT) {
      return BAD_FACE;
    }

    face_type = LEFT_BOTTOM;
  }

  if (v4 == bad_depth) {
    if (face_type != ALL_RIGHT) {
      return BAD_FACE;
    }

    face_type = RIGHT_BOTTOM;
  }

  return face_type;
}

bool DepthMesh::IsCorrectDepth(const double &depth) const {
  return ( (depth >= min_) && (depth <= max_) );
}

}  // namespace core
