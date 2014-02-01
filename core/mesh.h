// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_MESH_H_
#define CORE_MESH_H_

#include <vector>

#include "core/point3d.h"

namespace core {

class Triangle {
 public:
  Triangle(const int& point1, const int& point2, const int& point3) :
    point1_(point1),
    point2_(point2),
    point3_(point3) {}

  int Point1() const { return point1_; }
  int Point2() const { return point2_; }
  int Point3() const { return point3_; }

 private:
  int point1_;
  int point2_;
  int point3_;
};

class Mesh {
 public:
  Mesh();

  void AddVertex(const Point3D& point);
  void AddFace(const Triangle& face);

  std::vector<Point3D> Vertexes() const { return vertexes_; }
  std::vector<Triangle> Faces() const { return faces_; }

 protected:
  void SetVertexes(const std::vector<Point3D> vertexes);
  void SetFaces(const std::vector<Triangle> faces);

 private:
  std::vector<Point3D> vertexes_;
  std::vector<Triangle> faces_;
};

}  // namespace core

#endif  // CORE_MESH_H_
