// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Mlodik Mikhail <mlodik_m@mail.ru>

#include "core/mesh_merge_utils.h"

#include <vector>

namespace core {

void TopDownMerge(std::vector<int>* old_order,
                  int begin,
                  int middle,
                  int end,
                  std::vector<int>* new_order,
                  const std::vector<Point3D>& vertexes) {
  int i0 = begin;
  int i1 = middle;
  for (int j = begin; j < end; j++) {
      if ((i0 < middle) && ((i1 >= end) ||
        (vertexes.at(old_order->at(i0)).x() <
         vertexes.at(old_order->at(i1)).x()))) {
          new_order->at(j) = old_order->at(i0);
          i0 = i0 + 1;
      } else {
          new_order->at(j) = old_order->at(i1);
          i1 = i1 + 1;
      }
  }
  for (int j = begin; j < end; j++)
    old_order->at(j) = new_order->at(j);
}

void TopDownSplitMerge(std::vector<int>* old_order,
                       int begin,
                       int end,
                       std::vector<int>* new_order,
                       std::vector<Point3D> vertexes) {
  if ((end - begin) < 2) return;
  int middle = (begin + end) / 2;
  TopDownSplitMerge(old_order, begin, middle, new_order, vertexes);
  TopDownSplitMerge(old_order, middle, end, new_order, vertexes);
  TopDownMerge(old_order, begin, middle, end, new_order, vertexes);
}

void TopDownMergeSort(std::vector<int>* old_order,
                      std::vector<int>* new_order,
                      int n,
                      std::vector<Point3D> vertexes) {
  TopDownSplitMerge(old_order, 0, n, new_order, vertexes);
}

Mesh MergeSortByX(const Mesh& mesh) {
  int n = mesh.Vertexes().size();
  std::vector<int> old_order;
  std::vector<int> new_order;
  for (int i = 0; i < n; i++) {
    old_order.push_back(i);
    new_order.push_back(i);
  }
  TopDownMergeSort(&old_order, &new_order, n, mesh.Vertexes());
  Mesh new_mesh;
  for (int i = 0; i < n; i++)
    new_mesh.AddVertex(mesh.Vertexes()[new_order[i]]);
  for (int i = 0; i < mesh.Faces().size(); i++) {
    Triangle TmpFace(mesh.Faces()[new_order[i]].Point1(),
                     mesh.Faces()[new_order[i]].Point2(),
                     mesh.Faces()[new_order[i]].Point3());
    new_mesh.AddFace(TmpFace);
  }
  return new_mesh;
}

int BinarySearchByX(std::vector<Point3D> vertexes,
                    int begin,
                    int end,
                    double searched_value,
                    char sign) {
  // end is excluded, begin is included
  int result = -1;
  if ( end - begin == 1 ) return begin;
  if (sign == '<') {
    int middle = (begin + end) / 2;
    if (vertexes.at(middle).x() < searched_value) {
      result = BinarySearchByX(vertexes, middle, end, searched_value, sign);
    } else {
      result = BinarySearchByX(vertexes, begin, middle, searched_value, sign);
    }
  } else if (sign == '>') {
    int middle = (begin + end) / 2;
    if (vertexes.at(middle).x() < searched_value) {
      result = BinarySearchByX(vertexes, middle, end, searched_value, sign);
    } else {
      result = BinarySearchByX(vertexes, begin, middle, searched_value, sign);
    }
  } else { return -1; }
  return result;
}

}  // namespace core
