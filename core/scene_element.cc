// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "core/scene_element.h"

namespace core {

SceneElement::SceneElement() {
  SetStandardTransform();
}

SceneElement::SceneElement(Mesh *mesh) {
  SetStandardTransform();

  mesh_ = mesh;
}

void SceneElement::SetMesh(Mesh *mesh) {
  mesh_ = mesh;
}

void SceneElement::SetPos(double x, double y, double z) {
  pos_x_ = x;
  pos_y_ = y;
  pos_z_ = z;
}

void SceneElement::SetScale(double scale_x, double scale_y, double scale_z) {
  scale_x_ = scale_x;
  scale_y_ = scale_y;
  scale_z_ = scale_z;
}

std::vector<Point3D> SceneElement::Vertexes() const {
  std::vector<Point3D> new_vertexes;
  std::vector<Point3D> old_vertexes = mesh_->Vertexes();

  for (int i = 0; i < old_vertexes.size(); i++) {
    new_vertexes.push_back(Transform(old_vertexes.at(i)));
  }

  return new_vertexes;
}

std::vector<Triangle> SceneElement::Faces() const {
  return mesh_->Faces();
}

Point3D SceneElement::Transform(Point3D point) const {
  Point3D new_point;

  new_point.Set(point.x() + pos_x_, point.y() + pos_y_, point.z() + pos_z_);

  // DO LATER
  // Rotation and scaling
}

void SceneElement::SetStandardTransform() {
  pos_x_ = 0;
  pos_y_ = 0;
  pos_z_ = 0;

  scale_x_ = 1;
  scale_y_ = 1;
  scale_z_ = 1;

  rotation_x_ = 0;
  rotation_y_ = 0;
  rotation_z_ = 0;
}

}  // namespace core
