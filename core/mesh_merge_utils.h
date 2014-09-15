// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Mlodik Mikhail <mlodik_m@mail.ru>

#ifndef CORE_MESH_MERGE_UTILS_H_
#define CORE_MESH_MERGE_UTILS_H_

#include <vector>

#include "opencv2/core/core.hpp"
#include "core/mesh.h"

namespace core {

Mesh MergeSortByX(const Mesh& mesh);

enum Sign {
  MORE,
  LESS
};

int BinarySearchByX(std::vector<cv::Point3d> vertexes,
                    int begin,
                    int end,
                    double searched_value,
                    Sign sign);

}  // namespace core

#endif  // CORE_MESH_MERGE_UTILS_H_
