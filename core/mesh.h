// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_MESH_H_
#define CORE_MESH_H_

#include <vector>
#include "opencv2/core/mat.hpp"

namespace core {

class Triangle {
 public:
  Triangle(int point1, int point2, int point3) :
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
  void AddVertex(const cv::Point3d& point);
  void AddFace(const Triangle& face);

  const std::vector<cv::Point3d>& vertexes() const { return vertexes_; }
  const std::vector<Triangle>& faces() const { return faces_; }

  void ChangeVertex(const cv::Point3d& point, int i) {
    vertexes_.at(i) = point;
  }

 protected:
  void SetVertexes(const std::vector<cv::Point3d>& vertexes);
  void SetFaces(const std::vector<Triangle>& faces);

 private:
  std::vector<cv::Point3d> vertexes_;
  std::vector<Triangle> faces_;
};

}  // namespace core

#endif  // CORE_MESH_H_
