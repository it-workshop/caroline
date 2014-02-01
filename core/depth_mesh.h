// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_DEPTH_MESH_H_
#define CORE_DEPTH_MESH_H_

#include "core/mesh.h"
#include "core/depth_map.h"

namespace core {

class DepthMesh : public Mesh {
 public:
  DepthMesh(const DepthMap& depth_map, const int& min, const int& max);

 protected:
  enum FaceType {
    ALL_RIGHT = 0,
    BAD_FACE,
    LEFT_TOP,
    RIGHT_TOP,
    LEFT_BOTTOM,
    RIGHT_BOTTOM
  };

  void AddDepthVertex(const int& x, const int& y, const double& depth);
  void AddDepthFace(const int& v1, const int& v2, const int& v3);
  void AddDepthFaces(const int& v1,
    const int& v2, const int& v3, const int& v4);

  FaceType Type(const int& v1, const int& v2, const int& v3, const int& v4);

  bool IsCorrectDepth(const double& depth) const;

 private:
  double min_;
  double max_;
};

}  // namespace core

#endif  // CORE_DEPTH_MESH_H_
