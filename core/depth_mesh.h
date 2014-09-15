// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_DEPTH_MESH_H_
#define CORE_DEPTH_MESH_H_

#include "core/mesh.h"
#include "core/depth_map.h"

namespace core {

class DepthMesh : public Mesh {
 public:
  DepthMesh(const DepthMap& depth_map, int min, int max);

 protected:
  // For every four points verified can we connect them by triangles.
  // If all 4 points correct, then connect by two triangles.
  // If only one of the points is not correct, then connect the remaining.
  // If more then one is not correct, then we can not connect these points.
  enum FaceType {
    ALL_RIGHT = 0,
    BAD_FACE,
    LEFT_TOP,
    RIGHT_TOP,
    LEFT_BOTTOM,
    RIGHT_BOTTOM
  };

  void AddDepthVertex(int x, int y, double depth);
  void AddDepthFace(int v1, int v2, int v3);
  void AddDepthFaces(int v1, int v2, int v3, int v4);

  FaceType Type(int v1, int v2, int v3, int v4);

  bool IsCorrectDepth(double depth) const;

 private:
  double min_;
  double max_;
};

}  // namespace core

#endif  // CORE_DEPTH_MESH_H_
