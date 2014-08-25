// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "core/depth_mesh.h"

#include <vector>

namespace core {

DepthMesh::DepthMesh(const DepthMap& depth_map, int min, int max) {
  min_ = min;
  max_ = max;

  int height = depth_map.height();
  int width = depth_map.width();

  std::vector<int> vertex_counter_matrix;
  vertex_counter_matrix.resize(width * height);

  int counter = 0;

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      vertex_counter_matrix[width * i + j] = DepthMap::kBadDepth;

      double depth = depth_map.Depth(i, j);

      if (IsCorrectDepth(depth)) {
        vertex_counter_matrix[width * i + j] = counter;
        counter++;

        AddDepthVertex(i, j, depth);
      }
    }
  }

  for (int i = 0; i < height - 1; i++) {
    for (int j = 0; j < width - 1; j++) {
      AddDepthFaces(vertex_counter_matrix[width * i + j],
          vertex_counter_matrix[width * i + (j + 1)],
          vertex_counter_matrix[width * (i + 1) + j],
          vertex_counter_matrix[width * (i + 1) + (j + 1)]);
    }
  }
}

void DepthMesh::AddDepthVertex(int x, int y, double depth) {
  this->AddVertex(cv::Point3d(x, y, depth));
}

void DepthMesh::AddDepthFace(int v1, int v2, int v3) {
  this->AddFace(Triangle(v1, v2, v3));
}

void DepthMesh::AddDepthFaces(int v1, int v2, int v3, int v4) {
  FaceType type = Type(v1, v2, v3, v4);

  switch (type) {
    case ALL_RIGHT : {
      AddDepthFace(v1, v2, v4);
      AddDepthFace(v1, v3, v4);
      return;
    }
    case BAD_FACE : {
      return;
    }
    case LEFT_TOP : {
      AddDepthFace(v2, v3, v4);
      return;
    }
    case RIGHT_TOP : {
      AddDepthFace(v1, v3, v4);
      return;
    }
    case LEFT_BOTTOM : {
      AddDepthFace(v1, v2, v4);
      return;
    }
    case RIGHT_BOTTOM : {
      AddDepthFace(v1, v2, v3);
      return;
    }
  }
}

DepthMesh::FaceType DepthMesh::Type(int v1, int v2, int v3, int v4) {
  FaceType face_type = ALL_RIGHT;

  if (v1 == DepthMap::kBadDepth) {
    face_type = LEFT_TOP;
  }

  if (v2 == DepthMap::kBadDepth) {
    if (face_type != ALL_RIGHT) {
      return BAD_FACE;
    }

    face_type = RIGHT_TOP;
  }

  if (v3 == DepthMap::kBadDepth) {
    if (face_type != ALL_RIGHT) {
      return BAD_FACE;
    }

    face_type = LEFT_BOTTOM;
  }

  if (v4 == DepthMap::kBadDepth) {
    if (face_type != ALL_RIGHT) {
      return BAD_FACE;
    }

    face_type = RIGHT_BOTTOM;
  }

  return face_type;
}

bool DepthMesh::IsCorrectDepth(double depth) const {
  return ( (depth >= min_) && (depth <= max_) );
}

}  // namespace core
